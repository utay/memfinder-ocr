#include <limits.h>
#include <unistd.h>
#include <assert.h>
#include "../headers/detection.h"
#include "../headers/types.h"
#include "../headers/pixel_operations.h"
#include "../headers/preprocessing.h"

char classify(SDL_Surface *character);
char* ocr(SDL_Surface *image);
char detect_holes(SDL_Surface *character);
char has_hole(SDL_Surface *letter);
void fill(SDL_Surface *image, int x, int y, Uint32 color);
