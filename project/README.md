# Spidey

## Video Demo

https://youtu.be/fC5iJV5HScU

### Introduction

Spidey is a multithreaded web crawler written in the Go programming language.

A web crawler is an application used to index contents of the internet. It is primarily used by search engines to discover and index the web and provide improved results for user queries.
Unlike a standard crawler, Spidey only collects links to other pages or static content from a given webpage.

### An overview of how Spidey works

Spidey is run by giving a set of starting URLs or "seeds". It sends http GET requests to these seeds. If a successful response is recieved, Spidey crawls the content and discovers links to internet resources such as websites, images, etc.
It then repeats the process by sending requests to these new links. This process continues until the time specified by the user has elapsed. All of the links and errors are logged in text files.

### Usage

1. Create a "seeds.txt" in the project directory and add the seed links in quotes consecutively:

    Sample seeds.txt

```text
"http://example.com"
"https://abcd.com"
```

2. Build the project and run Spidey:
   Pass the total crawl time of the crawler and maximum request time per link (in milliseconds) as arguments

```bash
go build
./spidey 5000 2000
```

### Why Go was used

Go is a rapidly developing language that incorporates many impressive features from other languages. It has a unique concurrency model, utilizing goroutines as an abstraction over physical threads. It also includes useful synchronization primitives and features such as channels, which are data structures used to share data between goroutines.

### A deep dive into the application

#### main.go

The primary file of any Go program is "main.go". This file contains the functions that read "seeds.txt" and invoke the core crawling function, passing the seeds, total crawl time and maximum request time as parameters. The links are parsed from "seeds.txt" using regular expressions.

#### crawl.go

This file contains the core appliation logic. It has a public function CrawlLinks() and a public struct which stores the crawl statistics.
On invoking CrawlLinks(), the method creates a channel, which is used to pause the main goroutine until all of the child goroutines have terminated.
It then calls pingWebsites() by creating another goroutine.
The main routine is then blocked until the allowed crawl time has elapsed or if the child routine has been completed.
If the time has elapsed, it sets timeElapsed as true and waits for all child routines to terminate.

pingWebsites() takes in a set of urls and a channel.
Firstly, it checks if the time has elapsed by read locking the mutex.
If it has, an empty struct is returned to the channel, the timer mutex is read unlocked and the control returns to the parent routine.
Else, a channel is created and the totalLinkStore is locked.
The program loops over the given urls and checks if they have been already crawled, to prevent repeated operations.
If the link has not been crawled, the initial and trailing quotes are removed and the link is added to the totalLinkStore.
It then invokes fetchLinks() in a new goroutine, thus serving as the parent of several child goroutines.

fetchLinks() takes in a url and a channel.
It creates a new GET request to the given link.
If an error arises, an error is logged and stored in the totalErrorLogs mutex, a struct containing the failure message is passed to the channel and control is returned to the parent routine.
Before performing the http request, the timer mutex is read locked and examined.
If the total time has elapsed, an error is logged ,a struct containing the fetch status is passed to the channel and the mutex is read unlocked.
The goroutine then sleeps for 2 seconds. This delay has been enforced to reduce rate limit restrictions on websites.
The timer is then checked once again and the process is repeated.

A new goroutine is created which performs the GET request using Go's default http client.
The control then passes to the select block, which is a mechanism used to make a goroutine wait for multiple communication operations.
The select block is used to wait for a response from the url within the user specified maximum request time.
If a response is recieved, an empty struct is sent to the performRequestChan channel which removes the execution block on the current goroutine. It is checked for any errors.
If no response is recieved within the given time, an error is logged and the control passes back to the parent goroutine instance of pingWebsites()
The response sent by the url is read and checked for any errors.
If it is valid, all strings containing links in quotes are extracted using regular expressions.
These links are appended to single string slice.

fetchLinks() then creates a new channel and spawns a new parent goroutine instance of pingWwebsites(), passing the links crawled from the previous site and the newly created channel as arguments.
It then waits for data from the channel, thus blocking the current child goroutine until the newly created parent has been terminated.
Finally, fetchLinks() passes the crawl status as a struct to the calling parent goroutine's channel and returns control to the calling parent.

Thus, spidey works on this unique parent-child architecture where each child creates a new parent, thus creating a web of multiple goroutines.

pingWebsites() then recieves the crawl status sent by the child goroutines it had created, from the shared channels.
It adds this data to the totalCrawlStatus mutex and finally returns an empty struct to the main goroutine, before returning control to CrawlLinks()

CrawlLinks() then reads the cral statuses of all the urls and adds it to a public struct and to another struct to log the data.
This is passed to writeCrawlLogs() which the writes the data to a text file.
Then, writeErrorLogs() is invoked which records any errors encountered in a log.txt.
Finally, the function returns control to main.go and the application terminates.

#### crawl_test.go

This file contains the unit tests written to test spidey.
In fact, this was the first file created as spidey was built following the test-driven development approach.

TestPingWebsites() contains the code used to create dummy http servers using methods from Go's standard library.
It creates 'n' servers, with each server containing links to 'n' other http servers, where 'n' is a variable that can be modified to test spidey.
A total of n+(n*n) servers are created as a result of this test.
When the test is run, CrawlLinks() is invoked and the inital 'n' urls are passed as seeds.
The results are compared against the standard computed results.

createServer() is a helper function used to create http test servers on demand.
