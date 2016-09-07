#include "../headers/classification.h"
#include "../headers/stack.h"
#include "../headers/neural_network.h"

void fill(SDL_Surface *image, int x, int y, Uint32 color) {
  mystack *s = stack_init();
  stack_push(s, (coords_t) { x, y });
  do {
    coords_t c = stack_top(s);
    stack_pop(s);
    int x = c.x;
    int y = c.y;
    if (x >= 0 && y >= 0 && x < image->w && y < image->h &&
        getpixel(image, x, y) != color &&
        getpixel(image, x, y) != 65280) {
      putpixel(image, x, y, 65280);
      if (x > 0) {
        stack_push(s, (coords_t) { x - 1, y });
        if (y < image->h) {
          stack_push(s, (coords_t) { x - 1, y + 1 });
        }
        if (y > 0) {
          stack_push(s, (coords_t) { x - 1, y - 1 });
        }
      }
      if (x < image->w) {
        stack_push(s, (coords_t) { x + 1, y });
        if (y < image->h) {
          stack_push(s, (coords_t) { x + 1, y + 1 });
        }
        if (y > 0) {
          stack_push(s, (coords_t) { x + 1, y - 1 });
        }
      }
      if (y < image->h) {
        stack_push(s, (coords_t) { x, y + 1 });
      }
      if (y > 0) {
        stack_push(s, (coords_t) { x, y - 1 });
      }
    }
  } while (s->size != 0);
}

char has_hole(SDL_Surface *letter) {
  int wpixels = 0;
  for (int i = 0; i < letter->h; ++i) {
    if (getpixel(letter, 1, i) != 0) {
      fill(letter, 1, i, 0);
    }
  }
  for (int i = 0; i < letter->h; ++i) {
    if (getpixel(letter, letter->w - 1, i) != 0) {
      fill(letter, letter->w - 1, i, 0);
    }
  }
  for (int i = 0; i < letter->w; ++i) {
    if (getpixel(letter, i, 0) != 0) {
      fill(letter, i, 0, 0);
    }
  }
  for (int i = 0; i < letter->w; ++i) {
    if (getpixel(letter, i, letter->h -1) != 0) {
      fill(letter, i, letter->h - 1, 0);
    }
  }
  for (int j = 0; j < letter->h; ++j) {
    for (int i = 0; i < letter->w; ++i) {
      if (getpixel(letter, i, j) == 16777215) {
        wpixels++;
      }
    }
  }
  for (int j = 0; j < letter->h; ++j) {
    for (int i = 0; i < letter->w; ++i) {
      if (getpixel(letter, i, j) == 65280) {
        putpixel(letter, i, j, 16777215);
      }
    }
  }
  return wpixels > 10;
}

char is_letter_i(SDL_Surface *character) {
  int nb_wpixels = 0;
  for (int i = 0; i < character->w; ++i) {
    for (int j = 0; j < character->h; ++j) {
      if (getpixel(character, i, j) != 0) {
        ++nb_wpixels;
      }
    }
  }
  return nb_wpixels < character->h ? 1 : 0;
}

char comparison(SDL_Surface *character) {
  caracs_t caracs = get_caracs(character);
  neural_network(caracs);
  SDL_Surface* character_resized;
  char matched_character = '0';
  int min_diff = INT_MAX;
  for (int x = 0; x < 26; ++x) {
    if (x == 8) { //letter I
      continue;
    }
    int diff = 0;
    char *path;
    char root[255];
    assert(getcwd(root, 255) != NULL);
    if (asprintf(&path, "%s/extra/characters/%c2.png", root, 'A' + x) == -1) {
      printf("error with asprintf\n");
    }
    SDL_Surface* pattern = load_image(path);
    binarize(pattern);
    character_resized = scale_surface(character, pattern->w, pattern->h);
    for (int i = 0; i < character_resized->w; ++i) {
      for (int j = 0; j < character_resized->h; ++j) {
        if (getpixel(character_resized, i, j) != getpixel(pattern, i, j)) {
          ++diff;
        }
      }
    }
    //printf("%c : %d\n", 'A' + x, diff);
    if (diff < min_diff) {
      min_diff = diff;
      matched_character = 'A' + x;
    }
    free(path);
  }
  printf("pixppix %c\n\n", matched_character);
  display_image(character);
  return matched_character;
}

char classify(SDL_Surface *character) {
  if (is_letter_i(character)) {
    return 'I';
  }
  return comparison(character);
}

long color_average(SDL_Surface *image) {
  long sum = 0;
  int i = 0;
  int j = 0;
  for (i = 0; i < image->w; ++i) {
    for (j = 0; j < image->h; ++j) {
      sum += getpixel(image, i, j);
    }
  }
  sum /= i * j;
  return sum;
}

char* detect_characters(SDL_Surface *image) {
  int left[2] = { -1, -1 };
  int right[2] = { -1, -1 };
  int charac = 0;
  int nb_wpixels = 0;
  SDL_Surface **surfaces = malloc(sizeof(SDL_Surface*) * 64);
  char *characters = malloc(64);
  memset(characters, 0, 64);
  int count_spaces = 0;
  int spaces = 0;
  int first_space = 0;
  size_t i = 0;
  int w_average = 0;
  for (int x = 0; x < image->w; ++x) {
    for (int y = 0; y < image->h; ++y) {
      if (!charac) {
        if (getpixel(image, x, y) > 4500000) {
          left[0] = x;
          charac = 1;
          if (count_spaces) {
            spaces = x - spaces;
            if (!first_space) {
              first_space = spaces;
            } else if (spaces > first_space * 1.7) {
              characters[i] = ' ';
              ++i;
            }
            count_spaces = 0;
          }
        }
      } else {
        if (getpixel(image, x, y) > 4500000) {
          if (y < left[1] || left[1] == -1) {
            left[1] = y;
          }
          if (y > right[1] || right[1] == -1) {
            right[1] = y;
          }
          ++nb_wpixels;
        }
      }
    }
    if (nb_wpixels == 0) {
      if (charac) {
        right[0] = x;
        if (right[0] - left[0] < 10 || right[1] - left[1] < 10) {
          continue;
        }
        SDL_Surface *character = extract_image(image, left[0], left[1],
                                               right[0] - left[0], right[1] - left[1]);
        inverse(character);
        //display_image(character);
        w_average += character->w;
        characters[i] = classify(character);
        surfaces[i] = character;
        ++i;
        left[0] = left[1] = right[0] = right[1] = -1;
        charac = 0;
        spaces = x;
        count_spaces = 1;
      }
    }
    nb_wpixels = 0;
  }
  /* if (i != 0) { */
  /*   w_average /= i; */
  /* } */
  /* for (size_t j = 0; j < i; ++j) { */
  /*   if (characters[j] != ' ' && characters[j] != 'M' && characters[j] != 'W') { */
  /*     if (surfaces[j]->w > w_average * 1.6) { */
  /*       ++i; */
  /*       for (size_t k = i; k > j; --k) { */
  /*         characters[k] = characters[k - 1]; */
  /*         surfaces[k] = surfaces[k - 1]; */
  /*       } */
  /*       SDL_Surface *demi_charac = extract_image(surfaces[j], 0, 0, */
  /*                                                surfaces[j]->w / 2, surfaces[j]->h); */
  /*       characters[j] = classify(demi_charac); */
  /*       surfaces[j] = demi_charac; */
  /*       //display_image(demi_charac); */
  /*       demi_charac = extract_image(surfaces[j], surfaces[j]->w / 2, 0, */
  /*                                   surfaces[j]->w / 2, surfaces[j]->h); */
  /*       ++j; */
  /*       characters[j] = classify(demi_charac); */
  /*       surfaces[j] = demi_charac; */
  /*       //display_image(demi_charac); */
  /*     } */
  /*   } */
  /* } */
  return characters;
}

char* ocr(SDL_Surface *image) {
  SDL_Surface *org = SDL_CreateRGBSurface(SDL_HWSURFACE, image->w, image->h,
                                          image->format->BitsPerPixel,
                                          image->format->Rmask, image->format->Gmask,
                                          image->format->Bmask, image->format->Amask);
  for (int i = 0; i < image->w; ++i) {
    for (int j = 0; j < image->h; ++j) {
      putpixel(org, i, j, getpixel(image, i, j));
    }
  }
  display_image(org);
  convert_to_greyscale(image);
  blurring(image);
  SDL_Surface *edge_image = generate_edge_image(image);
  text_regions_t* TC = alloc_text_regions();
  TC = detect_text_regions(edge_image);
  display_image(edge_image);
  for (int k = 0; k < TC->size; ++k) {
    SDL_Surface* text_region = extract_image(edge_image, TC->regions[k].x0, TC->regions[k].y0,
                                         TC->regions[k].x1, TC->regions[k].y1);
    for (int i = 0; i < text_region->w; ++i) {
      for (int j = 0; j < text_region->h; ++j) {
        if (getpixel(text_region, i, j) > 5000000) {
          int x = i + TC->regions[k].x0;
          int y = j + TC->regions[k].y0;
          putpixel(org, x, y, 0);
          putpixel(org, x, y - 1, 0);
          putpixel(org, x, y + 1, 0);
          putpixel(org, x + 1, y, 0);
          putpixel(org, x + 1, y - 1, 0);
          putpixel(org, x + 1, y + 1, 0);
          putpixel(org, x - 1, y, 0);
          putpixel(org, x - 1, y - 1, 0);
          putpixel(org, x - 1, y + 1, 0);
        }
      }
    }
  }
  display_image(org);
  three_colors_image(org);
  display_image(org);
  for (int i = 1; i < image->w; ++i) {
    fill(org, i, 1, 16711680);
    fill(org, i, image->h - 1, 16711680);
  }
  display_image(org);
  for (int i = 0; i < org->w; ++i) {
    for (int j = 0; j < org->h; ++j) {
      if (getpixel(org, i, j) != 16777215) {
        putpixel(org, i, j, 0);
      }
    }
  }
  display_image(org);
  SDL_SaveBMP(org, "res");
  char* text = malloc(64);
  memset(text, 0, 64);
  SDL_Surface** text_regions = detect_text_zones(org);
  for (int i = 0; text_regions[i] != NULL; ++i) {
    display_image(text_regions[i]);
    strcat(text, detect_characters(text_regions[i]));
    if (text_regions[i + 1] != NULL) {
      strcat(text, "\n");
    }
  }
  return text;
}
