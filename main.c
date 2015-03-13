#include "sexpr.h"
#include <stdio.h>

int main() {
  struct reader_state *state = new_reader_state(stdin, stdout);
  struct cell *items = read_cell(state);

  while (items) {
    printf("> ");
    show_cell(items, state);
    free_cell(items);
    puts("\nok");

    items = read_cell(state);
  }

  free_reader_state(state);
  return 0;
}
