#include "../headers/stack.h"
#include <assert.h>

mystack* stack_init() {
  mystack *s = malloc(sizeof(mystack));
  s->size = 0;
  s->capacity = 1000;
  s->coords = malloc(sizeof(coords_t) * s->capacity);
  return s;
}

coords_t stack_top(mystack *s) {
  if (s->size == 0) {
    fprintf(stderr, "Error: stack empty\n");
    return (coords_t) { -1, -1 };
  }
  return s->coords[s->size - 1];
}

void stack_push(mystack *s, coords_t coords) {
  if (s->size == s->capacity) {
    s->capacity *= 2;
    s->coords = realloc(s->coords, sizeof(coords_t) * s->capacity);
  }
  s->coords[s->size++] = coords;
}

void stack_pop(mystack *s) {
  if (s->size == 0) {
    assert(0 && "Error: stack empty");
  } else {
    s->size--;
  }
}
