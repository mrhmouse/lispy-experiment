#include "read_show.h"
#include "constructors.h"
#include "string_builder.h"
#include <stdio.h>

static int is_whitespace(char c) {
  return c == ' '  ||
         c == '\t' ||
         c == '\n' ||
         c == '\r';
}

static int is_digit(char c) {
  return '0' <= c && c <= '9';
}

static int to_digit(char c) {
  return c - '0';
}

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

static struct cell *read_list() {
  return NULL; // TODO
}

static struct cell *read_symbol_from_sb(struct string_builder *s) {
  int c;
  while (1) {
    c = getchar();
    if (c == EOF || is_whitespace(c) || c == '(' || c == ')') {
      break;
    }

    sb_push_char(s, c);
  }

  return symbol_cell(sb_finalize(s));
}

static struct cell *read_symbol(char c) {
  struct string_builder *s = new_string_builder();
  sb_push_char(s, c);

  return read_symbol_from_sb(s);
}

static struct cell *continue_with_symbol(int val, char c) {
  struct string_builder *s = new_string_builder();
  while (val > 0) {
    sb_push_char(s, (val % 10) + '0');
    val = val / 10;
  }

  sb_push_char(s, '0');
  sb_push_char(s, c);

  return read_symbol_from_sb(s);
}

static struct cell *try_read_number(int val) {
  int c;

  while (1) {
    c = getchar();
    if (c == EOF) {
      return int_cell(val);
    }

    if (is_whitespace(c) || c == ')' || c == '(') {
      return int_cell(val);
    }

    if (is_digit(c)) {
      val = (val * 10) + to_digit(c);
      continue;
    }

    return continue_with_symbol(val, c);
  }
}

static struct cell *read_symbol_or_number(char c) {
  if (is_digit(c)) {
    return try_read_number(to_digit(c));
  }

  return read_symbol(c);
}

struct cell *read_cell() {
  int peek = getchar();
  if (peek == EOF) {
    return NULL;
  }

  if (is_whitespace(peek)) {
    return read_cell();
  }

  if (peek == '(') {
    return read_list();
  }

  return read_symbol_or_number(peek);
}
