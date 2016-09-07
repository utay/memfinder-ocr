#include "../headers/stack.h"
#include "../headers/preprocessing.h"
#include "../headers/classification.h"
#include "../headers/neural_network.h"
#include "../headers/pixel_operations.h"

void learning() {
  FILE *fp = fopen("learning", "w");
  if (fp == NULL) {
    printf("failed open learning");
    exit(-1);
  }
  for (int x = 0; x < 26; ++x) {
    char *path;
    if (asprintf(&path, "extra/characters/%c2.png", 'A' + x) == -1) {
      printf("error with asprintf\n");
    }
    SDL_Surface *character = load_image(path);
    caracs_t caracs = get_caracs(character);
    fprintf(fp, "%c : hor_sum30=%f hor_sum60=%f hor_sum100=%f | ver_sum30=%f ver_sum60=%f ver_sum100=%f | h_sym=%f v_sym=%f | hole=%d | avg=%f\n", 'A' + x,
            caracs.hor_sum30, caracs.hor_sum60, caracs.hor_sum100,
            caracs.ver_sum30, caracs.ver_sum60, caracs.ver_sum100,
            caracs.h_sym, caracs.v_sym, caracs.hole, caracs.avg);
    fflush(fp);
    free(path);
    SDL_FreeSurface(character);
  }
  fclose(fp);
}

void create_characters(int width, int height) {
  for (int x = 0; x < 26; ++x) {
    char *path;
    if (asprintf(&path, "extra/characters/%c2.png", 'A' + x) == -1) {
      printf("error with asprintf\n");
    }
    SDL_Surface *character = scale_surface(load_image(path), width, height);
    char *name;
    if (asprintf(&name, "%c", 'A' + x) == -1) {
      printf("error with asprintf\n");
    }
    SDL_SaveBMP(character, name);
    free(path);
    SDL_FreeSurface(character);
  }
}

int main() {
  learning();
  return 0;
}
