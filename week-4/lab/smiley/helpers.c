#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width]) {
  // Change all black pixels to a color of your choosing
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      BYTE *blue = &image[i][j].rgbtBlue;
      BYTE *green = &image[i][j].rgbtGreen;
      BYTE *red = &image[i][j].rgbtRed;
      if (*blue == 0 && *green == 0 && *red == 0) {
        *blue = 11;
        *green = 12;
        *red = 13;
      }
    }
  }
}
