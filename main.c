#include "sexpr.h"
#include <stdio.h>

int main() {
  struct cell *items =
    cons(
      symbol_cell("*"),
      cons(
        int_cell(2),
        cons(
          cons(
            symbol_cell("+"),
            cons(
              int_cell(3),
              cons(
                int_cell(4),
                nil()))),
          nil())));

  puts("\n----");
  show_cell(items);
  free_cell(items);
  puts("\n----");

  return 0;
}
