#include "constructors.h"
#include "macros.h"
#include <stdlib.h>

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
  /* Only the cell itself is freed.
     Any extra data (i.e. symbols)
     are left to the reader */
  if (c->type == C_PAIR) {
    free_cell(c->data.pair.head);
    free_cell(c->data.pair.tail);
  }

  free(c);
}
