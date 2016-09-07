#include <stdlib.h>
#include <stdio.h>
#include "../headers/types.h"
#include "../headers/classification.h"
#include "../headers/pixel_operations.h"

float horizontal_sum(SDL_Surface *image, int bot, int top);
float vertical_sum(SDL_Surface *image, int left, int right);
float h_symetry(SDL_Surface *image);
float v_symetry(SDL_Surface *image);
float average(float hor_sum30, float hor_sum60, float hor_sum100,
              float ver_sum30, float ver_sum60, float ver_sum100,
              float h_sym, float v_sym);
caracs_t get_caracs(SDL_Surface *character);
char neural_network(caracs_t c);
