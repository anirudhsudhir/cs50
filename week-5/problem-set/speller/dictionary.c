// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node {
  char word[LENGTH + 1];
  struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26 * 26;

bool dictionaryRead(const char *dictionary);
bool handleWord(const char *word);

bool tableCreated = false;
int wordsCount = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word) {
  // TODO
  int hashValue = hash(word);

  node *bucketPtr = table[hashValue];
  while (bucketPtr != NULL) {
    if (strcasecmp(bucketPtr->word, word) == 0) {
      return true;
    }
    bucketPtr = bucketPtr->next;
  }
  return false;
}

// Hashes word to a number
unsigned int hash(const char *word) {
  // TODO: Improve this hash function
  int i, j;
  int len = strlen(word);
  char str[46];
  char c = '\'';
  strcpy(str, word);
  for (i = j = 0; i < len; i++) {
    if (str[i] != c) {
      str[j++] = str[i];
    }
  }
  str[j] = '\0';

  int hashValue = 0;
  for (int k = 0; k < strlen(str); k++) {
    if (k > 2) {
      break;
    }
    hashValue += toupper(str[k]) - 'A';
  }
  return hashValue;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary) {
  // TODO
  return dictionaryRead(dictionary);
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void) {
  // TODO
  return wordsCount;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void) {
  // TODO
  for (int i = 0; i < N; i++) {
    node *tempBucketPtr = table[i];
    if (tempBucketPtr != NULL) {
      while (tempBucketPtr != NULL) {
        node *temp2 = tempBucketPtr->next;
        free(tempBucketPtr);
        tempBucketPtr = temp2;
      }
      free(tempBucketPtr);
    }
  }
  return true;
}

bool dictionaryRead(const char *dictionary) {
  FILE *file = fopen(dictionary, "r");
  if (file != NULL) {
    char *byte = malloc(1);
    if (byte != NULL) {
      byte[0] = 0;
      char *word = malloc(46);
      word[0] = 0;
      while (fread(byte, 1, 1, file) == 1) {
        if ((isalpha(*byte) || *byte == '\'') && *byte != '\n') {
          strncat(word, byte, 1);
        } else if (*byte == '\n') {
          strcat(word, "\0");
          if (handleWord(word) == false) {
            return false;
          }
          free(word);
          word = malloc(46);
          word[0] = 0;
        }
      }
      free(word);
      free(byte);
    }
  }
  fclose(file);
  return true;
}

bool handleWord(const char *word) {
  wordsCount++;
  if (tableCreated == false) {
    for (int i = 0; i < N; i++) {
      table[i] = NULL;
    }
    tableCreated = true;
  }
  int hashValue = hash(word);
  node *bucketPtr = table[hashValue];

  table[hashValue] = malloc(sizeof(node));
  if (table[hashValue] == NULL) {
    return false;
  }
  strcpy(table[hashValue]->word, "\0");
  strcpy(table[hashValue]->word, word);
  table[hashValue]->next = bucketPtr;

  return true;
}

