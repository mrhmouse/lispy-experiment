#include "debug_functions.h"
#include <stdio.h>

static void print_leaf(struct leaf l) {
  switch (l.type) {
    case L_INTEGER:
      printf("%i", l.data.integer);
      break;

    case L_FLOATING:
      printf("%f", l.data.floating);
      break;

    case L_SYMBOL:
      printf("%s", l.data.symbol);
      break;
  }
}

void print_cell(struct cell *c) {
  switch (c->type) {
    case C_NIL:
      printf("nil");
      break;

    case C_PAIR:
      printf("(");
      print_cell(c->data.pair.head);
      printf(" ");
      print_cell(c->data.pair.tail);
      printf(")");
      break;

    case C_LEAF:
      print_leaf(c->data.leaf);
      break;
  }
}
