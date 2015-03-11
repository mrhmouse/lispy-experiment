#pragma once
#include "data_types.h"

/** Print a cell on \p stdout */
void show_cell(struct cell *);

/** Read a cell from \p stdin */
struct cell *read_cell();
