int check(char *word, char first_time) {
  FILE *f = fopen("/usr/share/dict/words", "r");
  char line[20];
  while (fgets(line, 20, f) != NULL) {
    if (strlen(line) == strlen(word) && line[0] >= 97) {
      size_t i = 0;
      int j = -1;
      while (i < strlen(word)) {
        if (line[i] != word[i]) {
          if (j != -1) {
            j = -2;
            break;
          } else {
            j = i;
          }
        }
        ++i;
      }
      if (i == strlen(word) && j == -1) {
        return -1;
      } else {
        if (j != -2) {
          if (first_time) {
            return j;
          }
        }
      }
    }
  }
  return -2;
}

char* check_word_dictionary(char *word) {
  // same_word will be returned
  // if no word in dictionnary match with word
  char *same_word = malloc(strlen(word));
  for (size_t i = 0; i < strlen(word); ++i) {
    same_word[i] = word[i];
  }

  // check a first time
  FILE *f = fopen("/usr/share/dict/words", "r");
  char line[20];
  strcat(word, "\n");
  while (fgets(line, 20, f) != NULL) {
    if (strlen(line) == strlen(word)) {
      size_t i = 0;
      while (i < strlen(word)) {
        if (line[i] != word[i]) {
          break;
        }
        ++i;
      }
      if (i == strlen(word)) {
        return word;
      }
    }
  }
  int x = check(word, 1);
  if (x == -1) {
    return word;
  } else {
    char ch = 'a';
    for (int j = 0; j < 26; ++j) {
      word[x] = ch + j;
      if (check(word, 0) == -1) {
        return word;
      }
    }
  }
  return same_word;
}

char* verification(char *text) {
  char *result = malloc(strlen(text));
  memset(result, 0, strlen(text));
  int i = 0;
  while (text[i] != '\0') {
    if (text[i] > 64 && text[i] < 91) {
      text[i] += 32;
    }
    ++i;
  }
  printf("text = %s\n", text);
  i = 0;
  int old_i = 0;
  for (;;) {
    if (text[i] == ' ' || text[i] == '\0') {
      char *word = malloc(64);
      memset(word, 0, 64);
      for (int j = old_i != 0 ? old_i + 1 : 0; j < i; ++j) {
        word[j] = text[j];
        printf("%c\n", word[j]);
      }
      if (text[i] == '\0') {
        break;
      }
      printf("word[%zu] = %s\n", strlen(word), word);
      old_i = i;
    }
    ++i;
  }
  return result;
}
