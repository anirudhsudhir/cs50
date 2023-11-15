#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      BYTE *blue = &image[i][j].rgbtBlue;
      BYTE *green = &image[i][j].rgbtGreen;
      BYTE *red = &image[i][j].rgbtRed;
      BYTE average = round((*blue + *green + *red) / 3.0);
      *blue = average;
      *green = average;
      *red = average;
    }
  }
  return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width / 2; j++) {
      BYTE blue = image[i][j].rgbtBlue;
      BYTE green = image[i][j].rgbtGreen;
      BYTE red = image[i][j].rgbtRed;
      image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
      image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
      image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
      image[i][width - j - 1].rgbtBlue = blue;
      image[i][width - j - 1].rgbtGreen = green;
      image[i][width - j - 1].rgbtRed = red;
    }
  }
  return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE image_blur[height][width];
  double avgRed, avgGreen, avgBlue;
  int counter;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      counter = 0;
      avgRed = 0;
      avgGreen = 0;
      avgBlue = 0;
      for (int s = -1; s < 2; s++) {
        for (int n = -1; n < 2; n++) {
          int d = i + s;
          int k = j + n;
          if (d < height && k < width && d >= 0 && k >= 0) {
            avgRed = avgRed + image[d][k].rgbtRed;
            avgGreen = avgGreen + image[d][k].rgbtGreen;
            avgBlue = avgBlue + image[d][k].rgbtBlue;
            counter++;
          }
        }
      }
      image_blur[i][j].rgbtRed = round(avgRed / counter);
      image_blur[i][j].rgbtGreen = round(avgGreen / counter);
      image_blur[i][j].rgbtBlue = round(avgBlue / counter);
    }
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      image[i][j] = image_blur[i][j];
    }
  }
  return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width]) {
  RGBTRIPLE image_edge[height][width];
  double gxred = 0, gyred = 0, gxblue = 0, gyblue = 0, gxgreen = 0, gygreen = 0;
  int counter, valx, valy;

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      counter = 0;
      gxred = 0, gyred = 0, gxblue = 0, gyblue = 0, gxgreen = 0, gygreen = 0;
      for (int s = -1; s < 2; s++) {
        for (int n = -1; n < 2; n++) {
          int d = i + s;
          int k = j + n;
          if (d < height && k < width && d >= 0 && k >= 0) {
            if (s == -1 && n == -1) {
              valx = -1;
              valy = -1;
            } else if (s == -1 && n == 0) {
              valx = 0;
              valy = -2;
            } else if (s == -1 && n == 1) {
              valx = 1;
              valy = -1;
            } else if (s == 0 && n == -1) {
              valx = -2;
              valy = 0;
            } else if (s == 0 && n == 0) {
              valx = 0;
              valy = 0;
            } else if (s == 0 && n == 1) {
              valx = 2;
              valy = 0;
            } else if (s == 1 && n == -1) {
              valx = -1;
              valy = 1;
            } else if (s == 1 && n == 0) {
              valx = 0;
              valy = 2;
            } else if (s == 1 && n == 1) {
              valx = 1;
              valy = 1;
            }
            gxred += image[d][k].rgbtRed * valx;
            gyred += image[d][k].rgbtRed * valy;
            gxgreen += image[d][k].rgbtGreen * valx;
            gygreen += image[d][k].rgbtGreen * valy;
            gxblue += image[d][k].rgbtBlue * valx;
            gyblue += image[d][k].rgbtBlue * valy;
            counter++;
          }
        }
      }
      double totalred = pow(gxred, 2) + pow(gyred, 2);
      int temp = round(sqrt(totalred));
      if (temp > 255) {
        image_edge[i][j].rgbtRed = 255;
      } else {
        image_edge[i][j].rgbtRed = temp;
      }
      double totalgreen = pow(gxgreen, 2) + pow(gygreen, 2);
      temp = round(sqrt(totalgreen));
      if (temp > 255) {
        image_edge[i][j].rgbtGreen = 255;
      } else {
        image_edge[i][j].rgbtGreen = temp;
      }
      double totalblue = pow(gxblue, 2) + pow(gyblue, 2);
      temp = round(sqrt(totalblue));
      if (temp > 255) {
        image_edge[i][j].rgbtBlue = 255;
      } else {
        image_edge[i][j].rgbtBlue = temp;
      }
    }
  }
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      image[i][j] = image_edge[i][j];
    }
  }
  return;
}
