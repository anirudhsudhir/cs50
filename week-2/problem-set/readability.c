#include "cs50.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sen(string text);
int calculate_grade(int letters, int words, int sen);

int main(void) {
  string text = get_string("Text: ");
  // Calculating grade by finding number of letters, words and sentences
  int grade =
      calculate_grade(count_letters(text), count_words(text), count_sen(text));
  // Printing grade according to given ranges
  if (grade < 1) {
    printf("Before Grade 1");
  } else if (grade >= 16) {
    printf("Grade 16+");
  } else {
    printf("Grade %i", grade);
  }
  printf("\n");
}

int count_letters(string text) {
  int letters = 0;
  for (int i = 0; i < strlen(text); i++) {
    // Incrementing the sum if the character of the string is an alphabet
    if (isalpha(text[i])) {
      letters++;
    }
  }
  return letters;
}

int count_words(string text) {
  // Setting words to 1 as for n words, there are (n-1) whitespaces in between
  int words = 1;
  for (int i = 0; i < strlen(text); i++) {
    if (text[i] == ' ') {
      words++;
    }
  }
  return words;
}

int count_sen(string text) {
  int sentences = 0;
  for (int i = 0; i < strlen(text); i++) {
    char tempchar = text[i];
    // Checking if a string has begun by validating the end of line character
    if (tempchar == '?' || tempchar == '.' || tempchar == '!') {
      sentences++;
    }
  }
  return sentences;
}

int calculate_grade(int letters, int words, int sen) {
  // Calculating average number of letters for 100 words
  float L = (float)letters / words * 100;
  // Calculating average number of sentences for 100 words
  float S = (float)sen / words * 100;
  // Computing the Coleman-Liau index
  int index = round(0.0588 * L - 0.296 * S - 15.8);
  return index;
}
