#include "pixel_operations.h"
#include <math.h>

void convert_to_greyscale(SDL_Surface *surface);
void binarize_otsu(SDL_Surface *s);
SDL_Surface* extract_image(SDL_Surface *image, int positionX, int positionY,
                           int largeur, int hauteur);
SDL_Surface* scale_surface(SDL_Surface *Surface, Uint16 Width, Uint16 Height);
SDL_Surface* blurring(SDL_Surface* image);
SDL_Surface* sharpen(SDL_Surface* image);
void binarize(SDL_Surface* image);
void inverse(SDL_Surface *image);
void remove_noise(SDL_Surface *image);
