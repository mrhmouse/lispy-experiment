#include "read_show.h"
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

static void print_list(struct pair p) {
  show_cell(p.head);
  putchar(' ');

  struct cell *c = p.tail;

  while (c) {
    switch (c->type) {
      case C_NIL: return;

      case C_LEAF:
        printf(" . ");
        print_leaf(c->data.leaf);
        break;

      case C_PAIR:
        show_cell(c->data.pair.head);
        c = c->data.pair.tail;
        if (c->type != C_NIL) {
          putchar(' ');
        }
        break;
    }
  }
}

void show_cell(struct cell *c) {
  switch (c->type) {
    case C_NIL:
      printf("nil");
      break;

    case C_PAIR:
      putchar('(');
      print_list(c->data.pair);
      putchar(')');
      break;

    case C_LEAF:
      print_leaf(c->data.leaf);
      break;
  }
}
