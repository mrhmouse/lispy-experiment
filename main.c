#include "sexpr.h"
#include <stdio.h>

int main() {
  struct cell *items = read_cell();

  while (items) {
    printf("> ");
    show_cell(items);
    free_cell(items);
    puts("\nok");

    items = read_cell();
  }

  return 0;
}
