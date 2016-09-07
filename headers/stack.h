#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int x;
  int y;
} coords_t;

typedef struct {
  coords_t *coords;
  int size;
  int capacity;
} mystack;

mystack* stack_init();
coords_t stack_top(mystack *s);
void stack_push(mystack *s, coords_t coords);
void stack_pop(mystack *s);
