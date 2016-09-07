#ifndef _TYPES_H
# define _TYPES_H

#include <stdlib.h>

typedef struct {
  int* values;
  int size;
} histogram_t;

typedef struct {
  int x0;
  int x1;
  int y0;
  int y1;
} text_region_t;

typedef struct {
  text_region_t* regions;
  int size;
} text_regions_t;

histogram_t init_histogram(int size);
text_region_t init_text_region();
text_regions_t* alloc_text_regions();
void add_text_region(text_region_t tr, text_regions_t* trs);

/* Artificial neural network */

typedef struct {
  float hor_sum30;
  float hor_sum60;
  float hor_sum100;
  float ver_sum30;
  float ver_sum60;
  float ver_sum100;
  float h_sym;
  float v_sym;
  int hole;
  float avg;
} caracs_t;

#endif
