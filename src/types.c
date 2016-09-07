#include "../headers/types.h"

histogram_t init_histogram(int size) {
  histogram_t histogram;
  histogram.values = malloc(sizeof(int) * size);
  histogram.size = size;
  return histogram;
}

text_region_t init_text_region() {
  text_region_t tr;
  tr.x0 = 0;
  tr.x1 = 0;
  tr.y0 = 0;
  tr.y1 = 0;
  return tr;
}

text_regions_t* alloc_text_regions() {
  text_regions_t* trs = malloc(sizeof(text_regions_t));
  trs->regions = NULL;
  trs->size = 0;
  return trs;
}

void add_text_region(text_region_t tr, text_regions_t* trs) {
  trs->regions = realloc(trs->regions, sizeof(text_region_t) * (trs->size + 1));
  trs->regions[trs->size] = tr;
  ++(trs->size);
}
