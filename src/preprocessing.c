#include "../headers/preprocessing.h"

# define MAX_BRIGHTNESS 255
# define L 256
# define max(a,b) (a >= b ? a : b)
# define min(a,b) (a <= b ? a : b)

//convert an image to black and white
void convert_to_greyscale(SDL_Surface *surface) {
  Uint8 r, g, b;
  Uint16 grey_scale;
  Uint32 grey_pixel;
  Uint32 pixel;
  for (int x = 0; x < surface->w; ++x) {
    for (int y = 0; y < surface->h; ++y) {
      pixel = getpixel(surface, x, y);
      SDL_GetRGB(pixel, surface->format, &r, &g, &b);
      grey_scale = 0.3 * r + 0.59 * g + 0.11 * b;
      grey_pixel = SDL_MapRGB(surface->format, grey_scale, grey_scale, grey_scale);
      putpixel(surface, x, y, grey_pixel);
    }
  }
}

//binarize an image
void binarize_otsu(SDL_Surface *s)
{
  SDL_Surface *new_s;
  float *hist = malloc(sizeof(float) * 256);
  int max_x, max_y;
  Uint8 r, g, b, R, G, B;
  max_x = s->w;
  max_y = s->h;
  new_s = SDL_CreateRGBSurface(SDL_HWSURFACE, s->w, s->h,
                               s->format->BitsPerPixel, s->format->Rmask, s->format->Gmask,
                               s->format->Bmask, s->format->Amask);
  int N = max_x * max_y;

  for (int i = 0; i <= 256; ++i)
    {
      hist[i] = 0.0;
    }


  for (int x = 0; x < max_x;++x)
    {
      for(int y = 0; y<max_y; y++)
        {
          Uint32 current = getpixel(s, x, y);
          SDL_GetRGB(current, s->format, &r, &g, &b);
          int graylevel = max(0.0, min(255.0, 0.299 * r + 0.587 * g + 0.114 * b));
          hist[graylevel] += 1;
        }
    }

  for (int i = 0; i < 256; ++i)
    {
      hist[i] = hist[i] / N;
    }

  float ut = 0;
  for (int i = 0; i < 256; ++i)
    {
      ut += i * hist[i];
    }

  int max_k = 0;
  int max_sigma_k_ = 0;
  for(int k = 0; k < 256; ++k)
    {
      float wk = 0;
      for(int i = 0; i <= k; ++i)
        wk += hist[i];

      float uk = 0;
      for (int i = 0; i <= k; ++i)
        uk += i * hist[i];

      float sigma_k = 0;
      if (wk != 0 && wk != 1)
        sigma_k = ((ut * wk-uk) * (ut * wk-uk)) / (wk * (1 - wk));
      if (sigma_k > max_sigma_k_)
        {
          max_k = k;
          max_sigma_k_ = sigma_k;
        }
    }

  for (int x = 0; x < max_x; ++x)
    {
      for(int y = 0; y < max_y; ++y)
        {
          Uint32 cur;
          cur = getpixel(s, x, y);
          SDL_GetRGB(cur, s->format, &R, &G, &B);
          int graylevel = max(0.0, min(255.0, 0.299 * R + 0.587 * G + 0.114 * B));
          if(graylevel < max_k)
            putpixel(new_s, x, y, SDL_MapRGB(s->format, 255, 255, 255));
          else
            putpixel(new_s, x, y, SDL_MapRGB(s->format, 0, 0, 0));
        }
    }

  SDL_SaveBMP(new_s,"result.bmp");
  free(hist);
  SDL_FreeSurface(new_s);
}

//extract a part of an image
SDL_Surface* extract_image(SDL_Surface *image, int positionX, int positionY,
                                  int largeur, int hauteur) {
  SDL_Rect positionBlit;
  positionBlit.x = 0;
  positionBlit.y = 0;

  SDL_Rect positionExtraction;
  positionExtraction.x = positionX;
  positionExtraction.y = positionY;
  positionExtraction.w = largeur;
  positionExtraction.h = hauteur;

  SDL_Surface *nouvelleImageTemp = SDL_CreateRGBSurface(SDL_HWSURFACE, largeur, hauteur, 32, 0, 0, 0, 0);
  SDL_Surface *nouvelleImage = NULL;

  SDL_BlitSurface(image, &positionExtraction, nouvelleImageTemp, &positionBlit);

  nouvelleImage = nouvelleImageTemp;

  if (nouvelleImage == NULL) {
    fprintf(stderr, "Impossible d'extraire l'image");
    exit(EXIT_FAILURE);
  }

  return nouvelleImage;
}

//rescale an image
SDL_Surface* scale_surface(SDL_Surface *Surface, Uint16 Width, Uint16 Height) {
  if (!Surface || !Width || !Height)
    return 0;

  SDL_Surface *_ret = SDL_CreateRGBSurface(Surface->flags, Width, Height,
                      Surface->format->BitsPerPixel, Surface->format->Rmask,
                      Surface->format->Gmask, Surface->format->Bmask,
                      Surface->format->Amask);

  double _stretch_factor_x = (double) Width / (double) Surface->w;
  double _stretch_factor_y = (double) Height / (double) Surface->h;

  for (Sint32 y = 0; y < Surface->h; y++)
    for (Sint32 x = 0; x < Surface->w; x++)
      for (Sint32 o_y = 0; o_y < _stretch_factor_y; ++o_y)
        for (Sint32 o_x = 0; o_x < _stretch_factor_x; ++o_x)
          putpixel(_ret, (Sint32) (_stretch_factor_x * x) + o_x,
                   (Sint32) (_stretch_factor_y * y) + o_y, getpixel(Surface, x, y));
  return _ret;
}

SDL_Surface* blurring(SDL_Surface* image) {
  SDL_Surface *target = SDL_CreateRGBSurface( SDL_SWSURFACE, image->w, image->h,
                                              image->format->BitsPerPixel,
                                              image->format->Rmask,
                                              image->format->Gmask,
                                              image->format->Bmask, 0);
  for (int i = 1; i < image->w - 1; ++i) {
    for (int j = 1; j < image->h - 1; ++j) {
      unsigned r = 0, g = 0, b = 0;
      for (int n = i - 1; n <= i + 1; ++n) {
        for (int m = j - 1; m <= j + 1; ++m) {
          Uint8 rt, gt, bt;
          SDL_GetRGB(getpixel(image, n, m), image->format, &rt, &gt, &bt);
          r += rt;
          g += gt;
          b += bt;
        }
      }
      r /= 9;
      g /= 9;
      b /= 9;
      Uint32 pixel = SDL_MapRGB(target->format, r, g, b);
      putpixel(target, i, j, pixel);
    }
  }
  return target;
}

void binarize(SDL_Surface *image) {
  for (int i = 0; i < image->w; ++i) {
    for (int j = 0; j < image->h; ++j) {
      if (getpixel(image, i, j) < 15000000) {
        putpixel(image, i, j, 0);
      }
    }
  }
}

void inverse(SDL_Surface *image) {
  for (int i = 0; i < image->w; ++i) {
    for (int j = 0; j < image->h; ++j) {
      if (getpixel(image, i, j) < 15000000) {
        putpixel(image, i, j, 16777215);
      } else {
        putpixel(image, i, j, 0);
      }
    }
  }
}

void remove_noise(SDL_Surface *image) {
  int nb_pixels_per_line[image->h];
  Uint32 color_threshold = 12000000;
  for (int j = 0; j < image->h; ++j) {
    nb_pixels_per_line[j] = 0;
    for (int i = 0; i < image->w; ++i) {
      if (getpixel(image, i, j) > color_threshold) {
        ++nb_pixels_per_line[j];
      }
    }
  }
  int threshold = 0;
  for (int i = 0; i < image->h; ++i) {
    threshold += nb_pixels_per_line[i];
  }
  threshold /= image->h;
  for (int i = 0; i < image->h; ++i) {
    if (nb_pixels_per_line[i] < threshold / 4) {
      for (int j = 0; j < image->w; ++j) {
        putpixel(image, j, i, 0);
      }
    }
  }
  int nb_pixels_per_columns[image->w];
  for (int j = 0; j < image->w; ++j) {
    nb_pixels_per_columns[j] = 0;
    for (int i = 0; i < image->h; ++i) {
      if (getpixel(image, j, i) > color_threshold) {
        ++nb_pixels_per_columns[j];
      }
    }
  }
  threshold = 0;
  for (int i = 0; i < image->w; ++i) {
    threshold += nb_pixels_per_columns[i];
  }
  threshold /= image->w;
  for (int i = 0; i < image->w; ++i) {
    if (nb_pixels_per_columns[i] < threshold / 5) {
      for (int j = 0; j < image->h; ++j) {
        putpixel(image, i, j, 0);
      }
    }
  }
}
