#pragma once
#include "data_types.h"

/**
 \brief Create a cell holding an integer \p value.

 \param value The value that the cell will hold.
 \return A new cell.
*/
struct cell *int_cell(int value);

/**
 \brief Create a cell holding a double \p value.

 \param value The value that the cell will hold.
 \return A new cell.
*/
struct cell *double_cell(double value);

/**
 \brief Create a cell holding a symbol \p value.

 \param value The value that the cell will hold.
 \return A new cell.
*/
struct cell *symbol_cell(char *value);

/**
  \brief Create a cell holding no value.

  \return A new cell.
*/
struct cell *nil();

/**
  \brief Create a cell holding two other cells.

  \param head The first cell of the pair.
  \param tail The second cell of the pair.
  \return A new cell.
*/
struct cell *cons(struct cell *head, struct cell *tail);

/**
 \brief Free a cell completely, including everything
        that it references.

 \param cell The cell to free.
*/
void free_cell(struct cell *cell);
