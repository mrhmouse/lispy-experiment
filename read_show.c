#include "read_show.h"
#include "constructors.h"
#include "string_builder.h"
#include "macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct reader_state {
  FILE *input;
  FILE *output;

  size_t symbol_count;
  char **symbols;

  /* TODO: use a single area
     to hold incoming symbols */
};

char *intern_symbol(char *sym, struct reader_state *state) {
  size_t i;
  for (i = 0; i < state->symbol_count; ++i) {
    char *interned = state->symbols[i];
    if (0 == strcmp(interned, sym)) {
      free(sym);
      return interned;
    }
  }

  ++(state->symbol_count);
  RESIZE(state->symbols, state->symbol_count);
  state->symbols[state->symbol_count - 1] = sym;
  return sym;
}

struct reader_state *new_reader_state(FILE *input, FILE *output) {
  struct reader_state *state;
  NEW(state);

  state->input = input;
  state->output = output;
  state->symbol_count = 0;
  state->symbols = NULL;

  return state;
}

static void putback(int c, struct reader_state *state) {
  if (c == '(' || c == ')') {
    ungetc(c, state->input);
  }
}

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

static void print_string(char *str) {
  int i = 0;
  char c;

  putchar('"');

  while (1) {
    c = str[i++];
    if (c == '\0') {
      break;
    }

    switch (c) {
      case '\n':
        printf("\\n");
        break;

      case '\t':
        printf("\\t");
        break;

      case '\\':
        printf("\\\\");
        break;

      case '"':
        printf("\\\"");
        break;

      default:
        putchar(c);
        break;
    }
  }

  putchar('"');
}

static int contains_whitespace(char *str) {
  int i = 0;
  char c;

  while (1) {
    c = str[i++];

    if (c == '\0') {
      break;
    }

    if (is_whitespace(c)) {
      return 1;
    }
  }

  return 0;
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
      if (contains_whitespace(l.data.symbol)) {
        print_string(l.data.symbol);
      } else {
        printf("%s", l.data.symbol);
      }
      break;
  }
}

static void print_list(struct pair p,
    struct reader_state *state) {
  show_cell(p.head, state);
  if (p.tail->type != C_NIL) {
    putchar(' ');
  }

  struct cell *c = p.tail;

  while (c) {
    switch (c->type) {
      case C_NIL: return;

      case C_LEAF:
        printf(" . ");
        print_leaf(c->data.leaf);
        break;

      case C_PAIR:
        show_cell(c->data.pair.head, state);
        c = c->data.pair.tail;
        if (c->type != C_NIL) {
          putchar(' ');
        }
        break;
    }
  }
}

void show_cell(struct cell *c, struct reader_state *state) {
  switch (c->type) {
    case C_NIL:
      printf("nil");
      break;

    case C_PAIR:
      putchar('(');
      print_list(c->data.pair, state);
      putchar(')');
      break;

    case C_LEAF:
      print_leaf(c->data.leaf);
      break;
  }
}

static struct cell *read_list(struct reader_state *state);

static struct cell *read_symbol_from_sb(struct string_builder *s,
    struct reader_state *state) {
  int c;
  while (1) {
    c = fgetc(state->input);
    if (c == EOF || is_whitespace(c) || c == '(' || c == ')') {
      putback(c, state);
      break;
    }

    sb_push_char(s, c);
  }

  return symbol_cell(intern_symbol(sb_finalize(s), state));
}

static struct cell *read_string(
    struct reader_state *state) {
  struct string_builder *s = new_string_builder();
  int c;
  int escaping = 0;

  while (1) {
    c = fgetc(state->input);
    if (c == EOF) {
      break;
    }

    if (escaping) {
      switch (c) {
        case 'n':
          sb_push_char(s, '\n');
          break;

        case 't':
          sb_push_char(s, '\t');
          break;

        default:
          sb_push_char(s, c);
          break;
      }

      escaping = 0;
      continue;
    }

    switch (c) {
      case '\\':
        escaping = 1;
        break;

      case '"':
        goto done;

      default:
        sb_push_char(s, c);
        break;
    }
  }

done:
  return symbol_cell(intern_symbol(sb_finalize(s), state));
}

static struct cell *read_symbol(char c,
    struct reader_state *state) {
  if (c == '"') {
    return read_string(state);
  }

  struct string_builder *s = new_string_builder();
  sb_push_char(s, c);

  return read_symbol_from_sb(s, state);
}

static struct cell *continue_with_symbol(int val, char c,
    struct reader_state *state) {
  struct string_builder *s = new_string_builder();
  while (val > 0) {
    sb_push_char(s, (val % 10) + '0');
    val = val / 10;
  }

  sb_push_char(s, '0');
  sb_push_char(s, c);

  return read_symbol_from_sb(s, state);
}

static struct cell *try_read_number(int val,
    struct reader_state *state) {
  int c;
  // TODO: actually parse floating point

  while (1) {
    c = fgetc(state->input);
    if (c == EOF) {
      return int_cell(val);
    }

    if (is_whitespace(c) || c == ')' || c == '(') {
      putback(c, state);
      return int_cell(val);
    }

    if (is_digit(c)) {
      val = (val * 10) + to_digit(c);
      continue;
    }

    return continue_with_symbol(val, c, state);
  }
}

static struct cell *read_symbol_or_number(char c,
    struct reader_state *state) {
  if (is_digit(c)) {
    return try_read_number(to_digit(c), state);
  }

  return read_symbol(c, state);
}

static struct cell *read_cell_with_peek(int peek,
    struct reader_state *state) {
  if (peek == EOF) {
    return NULL;
  }

  if (is_whitespace(peek)) {
    return read_cell(state);
  }

  if (peek == '(') {
    return read_list(state);
  }

  return read_symbol_or_number(peek, state);
}

struct cell *read_cell(struct reader_state *state) {
  int peek = fgetc(state->input);
  return read_cell_with_peek(peek, state);
}

static struct cell *keep_reading_list(struct cell *list,
    struct reader_state *state) {
  int peek = fgetc(state->input);
  if (peek == EOF) {
    return cons(list, nil());
  }

  if (peek == ')') {
    return cons(list, nil());
  }

  if (is_whitespace(peek)) {
    return keep_reading_list(list, state);
  }

  struct cell *next = read_cell_with_peek(peek, state);
  return cons(list, keep_reading_list(next, state));
}

static struct cell *read_list(struct reader_state *state) {
  int peek = fgetc(state->input);
  if (peek == EOF) {
    return NULL;
  }

  if (peek == ')') {
    return nil();
  }

  if (is_whitespace(peek)) {
    return read_list(state);
  }

  struct cell *next = read_cell_with_peek(peek, state);
  return keep_reading_list(next, state);
}

void free_reader_state(struct reader_state *s) {
  size_t i;
  for (i = 0; i < s->symbol_count; ++i) {
    free(s->symbols[i]);
  }

  free(s->symbols);
  free(s);
}
