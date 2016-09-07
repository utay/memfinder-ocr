#include <limits.h>
#include "../headers/types.h"
#include "../headers/pixel_operations.h"
#include "../headers/preprocessing.h"

SDL_Surface* generate_edge_image(SDL_Surface *image);
histogram_t calculate_line_histogram(SDL_Surface* edge_image);
text_regions_t* determine_y_coordinate(histogram_t histogram);
text_regions_t* determine_x_coordinate(SDL_Surface* edge_image,text_regions_t* TC);
text_regions_t* detect_text_regions(SDL_Surface* edge_image);
char* detect_characters(SDL_Surface *image);
void three_colors_image(SDL_Surface *image);
SDL_Surface** detect_text_zones(SDL_Surface *image);
