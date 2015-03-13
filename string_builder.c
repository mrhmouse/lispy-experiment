#include "string_builder.h"
#include "macros.h"
#include <stdlib.h>

/* The default buffer size for
   string-builder structs */
#define DEFAULT_SIZE 32

/* Represents a string-builder,
   which pre-allocates memory
   as you push characters onto
   the end of it */
struct string_builder {
  size_t length;
  size_t buffer_size;
  char *buffer;
};

/* Initialize a new string-builder */
struct string_builder *new_string_builder() {
  struct string_builder *s;
  NEW(s);
  s->length = 0;
  s->buffer_size = DEFAULT_SIZE;
  s->buffer = NULL;
  RESIZE(s->buffer, DEFAULT_SIZE);
  return s;
}

/* Push a character onto the end of a string-builder,
   possibly resizing its internal buffer.

   This is the only place where the buffer can be
   increased. */
void sb_push_char(struct string_builder *s, char c) {
  size_t length = ++(s->length);
  if (length >= s->buffer_size) {
    s->buffer_size *= 2;
    RESIZE(s->buffer, s->buffer_size);
  }

  s->buffer[s->length - 1] = c;
}

/* Finalize a string-builder, returning its
   buffer (null-terminated).

   This is the only place where the buffer can
   be decreased. */
char *sb_finalize(struct string_builder *s) {
  sb_push_char(s, '\0');
  char *buf = s->buffer;
  RESIZE(buf, s->length);
  free(s);

  return buf;
}
