-- Keep a log of any SQL queries you execute as you solve the mystery.
-- Retrieving the schema of the database
.schema

-- Querying the database for the initial description the crime and any possible leads
select *
from crime_scene_reports
where year = 2021 and month = 7 and day = 28
;

-- Checking the transcript for clues
select transcript
from interviews
where year = 2021 and month = 7 and day = 28
;

-- Checking the list of people who exited the bakery within ten minutes of the robbery
-- using their license plates
select name
from people
where
    license_plate in (
        select license_plate
        from bakery_security_logs
        where
            year = 2021
            and month = 7
            and day = 28
            and hour = 10
            and minute >= 15
            and minute <= 25
    )
;

-- Checking the list of people who withdrew money from the ATM at Leggett Street on
-- the day of the theft
select name
from people
where
    id in (
        select person_id
        from bank_accounts
        where
            account_number in (
                select account_number
                from atm_transactions
                where
                    year = 2021
                    and month = 7
                    and day = 28
                    and atm_location = 'Leggett Street'
                    and transaction_type = 'withdraw'
            )
    )
;

-- Checking the list of people who made a call during the day of the theft of a
-- duration less than a minute
select name
from people
where
    phone_number in (
        select caller
        from phone_calls
        where year = 2021 and month = 7 and day = 28 and duration <= 60
    )
;

-- Checking the list of people who received a call during the day of the theft of a
-- duration less than a minute
select name
from people
where
    phone_number in (
        select receiver
        from phone_calls
        where year = 2021 and month = 7 and day = 28 and duration <= 60
    )
;

-- Checking the list of people who took the first flight on the day following the theft
select name
from people
where
    passport_number in (
        select passport_number
        from passengers
        where
            flight_id = (
                select id
                from flights
                where
                    year = 2021
                    and month = 7
                    and day = 29
                    and origin_airport_id
                    = (select id from airports where city = 'Fiftyville')
                order by hour
                limit 1
            )
    )
;


-- Finding the common person from the license plate, atm, call and flight queries
select name
from people
where
    license_plate in (
        select license_plate
        from bakery_security_logs
        where
            year = 2021
            and month = 7
            and day = 28
            and hour = 10
            and minute >= 15
            and minute <= 25
    )
intersect
select name
from people
where
    id in (
        select person_id
        from bank_accounts
        where
            account_number in (
                select account_number
                from atm_transactions
                where
                    year = 2021
                    and month = 7
                    and day = 28
                    and atm_location = 'Leggett Street'
                    and transaction_type = 'withdraw'
            )
    )
intersect
select name
from people
where
    phone_number in (
        select caller
        from phone_calls
        where year = 2021 and month = 7 and day = 28 and duration <= 60
    )
intersect
select name
from people
where
    passport_number in (
        select passport_number
        from passengers
        where
            flight_id = (
                select id
                from flights
                where year = 2021 and month = 7 and day = 29
                order by hour
                limit 1
            )
    )
;

-- Finding the destination of the flight the thief boarded
select city
from airports
where
    id = (
        select destination_airport_id
        from flights
        where year = 2021 and month = 7 and day = 29
        order by hour
        limit 1
    )
;

-- Finding the person who was called by Bruce on the day of the theft for less than a
-- minute
select name
from people
where
    phone_number = (
        select receiver
        from phone_calls
        where
            caller = (select phone_number from people where name = 'Bruce')
            and year = 2021
            and day = 28
            and month = 7
            and duration <= 60
    )
;
