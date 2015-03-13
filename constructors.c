#include "constructors.h"
#include <stdlib.h>
#define NEW(name) name = malloc(sizeof(*name))

static struct cell *leaf(struct leaf l) {
  struct cell *c;
  NEW(c);
  c->type = C_LEAF;
  c->data.leaf = l;
  return c;
}

struct cell *int_cell(int value) {
  struct leaf l = { L_INTEGER, .data.integer = value };
  return leaf(l);
}

struct cell *double_cell(double value) {
  struct leaf l = { L_FLOATING, .data.floating = value };
  return leaf(l);
}

struct cell *symbol_cell(char *value) {
  struct leaf l = { L_SYMBOL, .data.symbol = value };
  return leaf(l);
}

struct cell *nil() {
  struct cell *c;
  NEW(c);
  c->type = C_NIL;
  return c;
}

struct cell *cons(struct cell *head, struct cell *tail) {
  struct pair p = { head, tail };
  struct cell *c;
  NEW(c);
  c->type = C_PAIR;
  c->data.pair = p;
  return c;
}

void free_cell(struct cell *c) {
  switch (c->type) {
    case C_NIL: break;

    case C_PAIR:
      free_cell(c->data.pair.head);
      free_cell(c->data.pair.tail);
      break;

    case C_LEAF:
      if (c->data.leaf.type == L_SYMBOL) {
        free(c->data.leaf.data.symbol);
      }
      break;
  }

  free(c);
}
