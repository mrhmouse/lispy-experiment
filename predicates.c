#include "predicates.h"

int is_nil(struct cell c) {
  return c.type == C_NIL;
}
