#pragma once
#include <stdio.h>
#include "data_types.h"

struct reader_state;

struct reader_state *new_reader_state(FILE *input, FILE *output);

void free_reader_state(struct reader_state *);

void show_cell(struct cell *, struct reader_state *);

struct cell *read_cell(struct reader_state *);
