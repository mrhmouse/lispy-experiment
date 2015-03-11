#pragma once

/** Represents a pair of cells. */
struct pair {
  /** The first cell in the pair */
  struct cell *head;
  
  /** The second cell in the pair */
  struct cell *tail;
};

/** Represents a leaf node, which holds a value. */
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
