#include "cs50.h"
#include <stdio.h>

int main(void) {
  int dummy = get_int("Enter a number: ");
  int x = 1;
  int y = 3;
  float z = (float)x / y;
  printf("%f%d\n", z, dummy);
}
