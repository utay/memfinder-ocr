#include "../headers/neural_network.h"

#define ALPHABET_SIZE 26

float horizontal_sum(SDL_Surface *image, int bot, int top) {
  float sum = 0;
  for (int j = bot; j < top; ++j) {
    for (int i = 0; i < image->w; ++i) {
      if (getpixel(image, i, j) == 0) {
        ++sum;
      }
    }
  }
  sum /= (top - bot) * image->w;
  return sum;
}

float vertical_sum(SDL_Surface *image, int left, int right) {
  float sum = 0;
  for (int i = left; i < right; ++i) {
    for (int j = 0; j < image->h; ++j) {
      if (getpixel(image, i, j) == 0) {
        ++sum;
      }
    }
  }
  sum /= (right - left) * image->h;
  return sum;
}

float h_symetry(SDL_Surface *image) {
  float degre = 0;
  for (int j = 0; j < image->h / 2; ++j) {
    for (int i = 0; i < image->w; ++i) {
      if (getpixel(image, i, j) == getpixel(image, i, image->h - 1 - j)) {
        ++degre;
      }
    }
  }
  degre /= image->w * image->h / 2;
  return degre;
}

float v_symetry(SDL_Surface *image) {
  float degre = 0;
  for (int j = 0; j < image->h; ++j) {
    for (int i = 0; i < image->w / 2; ++i) {
      if (getpixel(image, i, j) == getpixel(image, image->w - 1 - i, j)) {
        ++degre;
      }
    }
  }
  degre /= image->w * image->h / 2;
  return degre;
}

float average(float hor_sum30, float hor_sum60, float hor_sum100,
              float ver_sum30, float ver_sum60, float ver_sum100,
              float h_sym, float v_sym) {
  return (hor_sum30 + hor_sum60 + hor_sum100 + ver_sum30 + ver_sum60 + ver_sum100 +
          h_sym + v_sym) / 8;
}

caracs_t get_caracs(SDL_Surface *character) {
  caracs_t caracs;
  caracs.hor_sum30 = horizontal_sum(character, 0, character->h / 3);
  caracs.hor_sum60 = horizontal_sum(character, character->h / 3, character->h / 3 * 2);
  caracs.hor_sum100 = horizontal_sum(character, character->h / 3 * 2, character->h);
  caracs.ver_sum30 = vertical_sum(character, 0, character->w / 3);
  caracs.ver_sum60 = vertical_sum(character, character->w / 3, character->w / 3 * 2);
  caracs.ver_sum100 = vertical_sum(character, character->w / 3 * 2, character->w);
  caracs.h_sym = h_symetry(character);
  caracs.v_sym = v_symetry(character);
  caracs.hole = has_hole(character);
  caracs.avg = (caracs.hor_sum30 + caracs.hor_sum60 + caracs.hor_sum100 +
                caracs.ver_sum30 + caracs.ver_sum60 + caracs.ver_sum100 +
                caracs.h_sym + caracs.v_sym) / 8;
  return caracs;
}

char neuron(float carac, float *patterns) {
  float min_diff = 1000;
  char character = 'A';
  for (int i = 0; i < ALPHABET_SIZE; ++i) {
    if (fabs(carac - patterns[i]) < min_diff) {
      min_diff = fabs(carac - patterns[i]);
      character = 'A' + i;
    }
  }
  return character;
}

char neural_network(caracs_t c) {
  FILE *fp = fopen("learning", "r");
  char line[255];
  char *characs = malloc(sizeof(char) * ALPHABET_SIZE);
  float *hor_sum30s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *hor_sum60s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *hor_sum100s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *ver_sum30s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *ver_sum60s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *ver_sum100s = malloc(sizeof(float) * ALPHABET_SIZE);
  float *h_syms = malloc(sizeof(float) * ALPHABET_SIZE);
  float *v_syms = malloc(sizeof(float) * ALPHABET_SIZE);
  int *holes = malloc(sizeof(int) * ALPHABET_SIZE);
  float *averages = malloc(sizeof(float) * ALPHABET_SIZE);
  int i = 0;
  while (fgets(line, 255, fp) != NULL) {
    sscanf(line, "%c : hor_sum30=%f hor_sum60=%f hor_sum100=%f | ver_sum30=%f ver_sum60=%f ver_sum100=%f | h_sym=%f v_sym=%f | hole=%d | avg=%f\n", &(characs[i]), &(hor_sum30s[i]), &(hor_sum60s[i]),
           &(hor_sum100s[i]), &(ver_sum30s[i]), &(ver_sum60s[i]), &(ver_sum100s[i]),
           &(h_syms[i]), &(v_syms[i]), &(holes[i]), &(averages[i]));
    ++i;
  }
  printf("hole    %d\n", c.hole);
  printf("hsum30  %c\n", neuron(c.hor_sum30, hor_sum30s));
  printf("hsum60  %c\n", neuron(c.hor_sum60, hor_sum60s));
  printf("hsum100 %c\n", neuron(c.hor_sum100, hor_sum100s));
  printf("vsum30  %c\n", neuron(c.ver_sum30, ver_sum30s));
  printf("vsum60  %c\n", neuron(c.ver_sum60, ver_sum60s));
  printf("vsum100 %c\n", neuron(c.ver_sum100, ver_sum100s));
  printf("hsym    %c\n", neuron(c.h_sym, h_syms));
  printf("vsym    %c\n", neuron(c.v_sym, v_syms));
  printf("avg     %c\n", neuron(c.avg, averages));
  return 'A';
}
