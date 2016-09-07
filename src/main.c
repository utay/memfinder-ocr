#include <stdlib.h>
#include <err.h>
#include <string.h>
#include <limits.h>
#include <stdio.h>
#include <assert.h>
#include "../headers/pixel_operations.h"
#include "../headers/preprocessing.h"
#include "../headers/types.h"
#include "../headers/detection.h"
#include "../headers/classification.h"
#include "../headers/neural_network.h"

void put_in_white(SDL_Surface *image)
{
    Uint32 color_white = 16645629;
    int white = 0;
    for (int j = 0; j < image->h; j++)
    {
        for (int i = 0; i < image->w; i++)
        {
            if (getpixel(image,i,j) > 0)
                printf("%d\n",getpixel(image,i,j));
            if ((white == 0) && (getpixel(image,i,j) > color_white))
                white = 1;
            else {
                if ((white == 1) && ( getpixel(image,i,j) < color_white))
                    putpixel(image,i,j,color_white);
                else {
                    if ((white == 1) && (getpixel(image,i,j) > color_white))
                        white =0;
                }

            }
        }
    }
}

void fill_per_pixels(SDL_Surface *image) {
  for (int i = 0; i < image->w; ++i) {
    for (int j = 0; j < image->h; ++j) {
      if (!getpixel(image, i, j) && i > 0 && j > 0 &&
          i < image->w && j < image->h) {
        if (getpixel(image, i + 1, j) && getpixel(image, i - 1, j) &&
            getpixel(image, i, j + 1) && getpixel(image, i, j - 1) &&
            getpixel(image, i + 1, j + 1) && getpixel(image, i - 1, j - 1) &&
            getpixel(image, i + 1, j - 1) && getpixel(image, i - 1, j + 1)) {
          putpixel(image, i, j, 16777215);
        }
      }
      if (getpixel(image, i, j) && i > 0 && j > 0 &&
          i < image->w && j < image->h) {
        if (!getpixel(image, i + 1, j) && !getpixel(image, i - 1, j) &&
            !getpixel(image, i, j + 1) && !getpixel(image, i, j - 1) &&
            !getpixel(image, i + 1, j + 1) && !getpixel(image, i - 1, j - 1) &&
            !getpixel(image, i + 1, j - 1) && !getpixel(image, i - 1, j + 1)) {
          putpixel(image, i, j, 0);
        }
      }
    }
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    err(3, "invalid arguments");
  }
  init_sdl();
  SDL_Surface *image = load_image(argv[1]);
  printf("%s\n", ocr(image));
  return 0;
}
