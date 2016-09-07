#include <limits.h>
#include "../headers/detection.h"
#include "../headers/types.h"
#include "../headers/pixel_operations.h"
#include "../headers/preprocessing.h"
#include "../headers/classification.h"

// link for algorithms:
// http://www.mathematik.uni-marburg.de/~ewerth/papers/ISPA2003.pdf

Uint32 max3(Uint32 a, Uint32 b, Uint32 c) {
  if (a >= b && a >= c) {
    return a;
  }
  if (b >= a && b >= c) {
    return b;
  }
  if (c >= a && c >= b) {
    return c;
  }
  return -1;
}

SDL_Surface* generate_edge_image(SDL_Surface *image) {
  Uint32 left, upper, rightUpper = 0;
  SDL_Surface *edgeImg;
  edgeImg = SDL_CreateRGBSurface(SDL_HWSURFACE, image->w, image->h,
                                 image->format->BitsPerPixel,
                                 image->format->Rmask,
                                 image->format->Gmask,
                                 image->format->Bmask,
                                 image->format->Amask);
  for (int x = 0; x < image->w; ++x) {
    for (int y = 0; y < image->h; ++y) {
      if (x > 0 && x < image->w - 1 && y > 0 && y < image->h) {
        left = abs(getpixel(image, x, y) - getpixel(image, x - 1, y));
        upper = abs(getpixel(image, x, y) - getpixel(image, x, y - 1));
        rightUpper = abs(getpixel(image, x, y) - getpixel(image, x + 1, y - 1));
        putpixel(edgeImg, x, y, max3(left, upper, rightUpper));
      } else {
        putpixel(edgeImg, x, y, 0);
      }
    }
  }
  return edgeImg;
}

histogram_t calculate_line_histogram(SDL_Surface* edge_image) {
  histogram_t histogram = init_histogram(edge_image->h);
  for (int y = 0; y < edge_image->h; ++y) {
    int pixelsOverValue = 0;
    for (int x = 0; x < edge_image->w; ++x) {
      if (getpixel(edge_image, x, y) >  4500000 /* given value 190000 */) {
        ++pixelsOverValue;
      }
    }
    histogram.values[y] = pixelsOverValue;
  }
  return histogram;
}

text_regions_t* determine_y_coordinate(histogram_t histogram) {
  text_region_t rect = init_text_region();
  text_regions_t* TC = alloc_text_regions();
  char inside_text_area = 0;
  for (int y = 0; y < histogram.size; ++y) {
    //printf("%d\n", histogram.values[y]);
    if (histogram.values[y] > 20 /* min edges */
        || (y > 0 && ((histogram.values[y] - histogram.values[y - 1]) > 0 /* min line diff */))) {
      if (!inside_text_area) {
        rect.y0 = y;
        inside_text_area = 1;
      }
    } else if (inside_text_area) {
      rect.y1 = y - 1;
      if ((rect.y1 - rect.y0) > 5 /* min lines */
          && (rect.y1 - rect.y0) < 200 /* max lines */) {
        add_text_region(rect, TC);
      }
      inside_text_area = 0;
    }
  }
  return TC;
}

text_regions_t* determine_x_coordinate(SDL_Surface* edge_image,
                                      text_regions_t* TC) {
  int left = INT_MAX;
  int right = -1;
  for (int i = 0; i < TC->size; ++i) {
    for (int x = 0; x < edge_image->w; ++x) {
      for (int y = TC->regions[i].y0; y < TC->regions[i].y1; ++y) {
        if (getpixel(edge_image, x, y) != 0) {
          if (left > x) {
            left = x;
          }
          if (right < x) {
            right = x;
          }
        }
      }
    }
    TC->regions[i].x0 = left;
    TC->regions[i].x1 = right;
  }
  return TC;
}

text_regions_t* detect_text_regions(SDL_Surface* edge_image) {
  histogram_t histogram = calculate_line_histogram(edge_image);
  text_regions_t* TC = determine_y_coordinate(histogram);
  TC = determine_x_coordinate(edge_image, TC);
  return TC;
}


void three_colors_image(SDL_Surface *image) {
  for (int i = 0; i < image->w; ++i) {
    for (int j = 0; j < image->h; ++j) {
      if (getpixel(image, i, j) > 15000000) {
        putpixel(image, i, j, 16777215);
      } else if (getpixel(image, i, j) < 1500000) {
        putpixel(image, i, j, 16711680);
      } else {
        putpixel(image, i, j, 0);
      }
    }
  }
}

SDL_Surface** detect_text_zones(SDL_Surface *image) {
  int top_pos = -1;
  int bot_pos = -1;
  int nb_text_regions = 0;
  SDL_Surface** text_regions = malloc(sizeof(SDL_Surface*) * 64);
  for (int j = 0; j < image->h; ++j) {
    char white = 0;
    for (int i = 0; i < image->w; ++i) {
      if (getpixel(image, i, j) != 0 && top_pos == -1) {
        top_pos = j;
      }
      if (getpixel(image, i, j) != 0) {
        white = 1;
      }
    }
    if (!white && top_pos != -1) {
      bot_pos = j;
      if (bot_pos - top_pos > 20) {
        text_regions[nb_text_regions++] = extract_image(image, 0, top_pos,
                                                        image->w, bot_pos - top_pos);
      }
      top_pos = -1;
      bot_pos = -1;
    }
  }
  text_regions[nb_text_regions] = NULL;
  return text_regions;
}
