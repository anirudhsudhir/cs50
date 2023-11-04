#include "cs50.h"
#include <stdio.h>

int main(void) {
  // TODO: Prompt for start size
  int start;
  do {
    start = get_int("Start: ");
  } while (start < 9);

  // TODO: Prompt for end size
  int stop;
  do {
    stop = get_int("Stop: ");
  } while (stop < start);

  // TODO: Calculate number of years until we reach threshold
  int livepop = start;
  int n = 0;
  while (livepop < stop) {
    n++;
    livepop += (livepop / 3 - livepop / 4);
  }

  // TODO: Print number of years
  printf("Years: %i\n", n);
}
