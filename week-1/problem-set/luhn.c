#include "cs50.h"
#include <math.h>
#include <stdio.h>

int main(void) {
  // Accepting input from user
  long num = get_long("Enter the card number: ");
  bool check = false;
  string card = "";
  int counter = 1, sum2 = 0, sumother = 0, sumall = 0;

  // Using Luhn's Algorithm
  for (long i = num; i > 0; i /= 10, counter++) {
    // Finding the sum of digits in even places multipled by two
    if (counter % 2 == 0) {
      int prod = (i % 10) * 2;
      for (int j = prod; j > 0; j /= 10) {
        sum2 += j % 10;
      }
    }
    // Finding the sum of digits in odd places
    else {
      sumother += i % 10;
    }
  }
  // Calculating the total sum of both sets of digits
  sumall = sum2 + sumother;

  if (sumall % 10 == 0) {
    check = true;
  }

  // Classifying card companies on the basis of card numbers
  if (check) {
    // Checking if card numbers begin with specified digits
    if ((int)(num / pow(10, 13)) == 34 || (int)(num / pow(10, 13)) == 37) {
      card = "AMEX";
    } else if ((int)(num / pow(10, 14)) == 51 ||
               (int)(num / pow(10, 14)) == 52 ||
               (int)(num / pow(10, 14)) == 53 ||
               (int)(num / pow(10, 14)) == 54 ||
               (int)(num / pow(10, 14)) == 55) {
      card = "MASTERCARD";
    } else if ((int)(num / pow(10, 12)) == 4 || (int)(num / pow(10, 15)) == 4) {
      card = "VISA";
    } else {
      card = "INVALID";
    }
  } else {
    card = "INVALID";
  }

  // Printing the validity and company of a card
  printf("%s\n", card);
}
