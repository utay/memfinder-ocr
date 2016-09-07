char hole_verification(SDL_Surface *character, int top, int bot) {
  for (int j = top; j < bot; ++j) {
    int hole = 0;
    char in_hole = 0;
    for (int i = 1; i < character->w; ++i) {
      if (getpixel(character, i, j) == 0 &&
          !in_hole && hole < 3) {
        ++hole;
        in_hole = 1;
      }
      if (getpixel(character, i, j) != 0 &&
          in_hole && hole < 3) {
        ++hole;
        in_hole = 0;
      }
    }
    if (hole != 3) {
      return 0;
    }
  }
  return 1;
}

char detect_holes(SDL_Surface *character) {
  char hole = 0;
  int top = 0;
  int bot = 0;
  for (int i = 1; i < character->h - 1; ++i) {
    for (int j = 1; j < character->w - 1; ++j) {
      if (!hole && getpixel(character, j, i) != 0 &&
          getpixel(character, j - 1, i) == 0 &&
          getpixel(character, j, i - 1) == 0) {
        for (int k = j + 1; k < character->w - 1; ++k) {
          if (getpixel(character, k, i) == 0) {
            top = i;
            hole = 1;
            break;
          }
        }
      }
      if (hole && i < character->h - 5 && j < character->w - 5) {
        if (getpixel(character, j, i) != 0 &&
            getpixel(character, j + 1, i) == 0 &&
            getpixel(character, j, i + 1) == 0) {
          for (int k = j + 1; k < character->w - 1; ++k) {
            if (getpixel(character, k, i) == 0) {
              bot = i;
              return hole_verification(character, top, bot);
            }
          }
          hole = 0;
        }
      }
    }
  }
  return 0;
}
