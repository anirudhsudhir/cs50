#include "cs50.h"
#include <stdio.h>

int main(void) {
  // Accepting size of pyramid from the user
  int size;
  do {
    size = get_int("Enter the size of the pyramid: ");
  } while (size < 1 || size > 8);

  // Printing the pyramid
  for (int i = 1; i <= size; i++) {
    for (int j = size - i; j >= 1; j--) {
      printf(" ");
    }
    for (int j = 1; j <= i; j++) {
      printf("#");
    }
    // Adding the space between the triangles
    printf("  ");
    for (int j = 1; j <= i; j++) {
      printf("#");
    }
    // Moving to the next line
    printf("\n");
  }
}
