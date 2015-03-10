#pragma once
#include "data_types.h"

struct cell int_cell(int value);
struct cell double_cell(double value);
struct cell symbol_cell(char *value);
struct cell nil();
struct cell cons(struct cell *, struct cell *);
