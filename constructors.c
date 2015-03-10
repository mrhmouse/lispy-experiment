#include "constructors.h"
#include <string.h>

struct cell int_cell(int value) {
  struct leaf l = { L_INTEGER, value };
  struct cell c = { C_LEAF, l };
  return c;
}

struct cell double_cell(double value) {
  struct leaf l = { L_FLOATING, value };
  struct cell c = { C_LEAF, l };
  return c;
}

struct cell symbol_cell(char *value) {
  struct leaf l = { L_SYMBOL, strdup(value) };
  struct cell c = { C_LEAF, l };
  return c;
}

struct cell nil() {
  struct cell c = { C_NIL, NULL };
  return c;
}

struct cell cons(struct cell *head, struct cell *tail) {
  struct pair p = { head, tail };
  struct cell c = { C_PAIR, p };
  return c;
}
