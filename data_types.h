#pragma once

struct pair {
  struct cell *head;
  struct cell *tail;
};

struct leaf {
  enum {
    L_INTEGER,
    L_FLOATING,
    L_SYMBOL
  } type;

  union {
    int integer;
    double floating;
    char *symbol;
  } data;
};

struct cell {
  enum {
    C_NIL,
    C_PAIR,
    C_LEAF
  } type;

  union {
    struct pair pair;
    struct leaf leaf;
  } data;
};
