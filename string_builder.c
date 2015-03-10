#include "string_builder.h"
#include <stdlib.h>

#define DEFAULT_SIZE 32

struct string_builder {
  size_t length;
  size_t buffer_size;
  char *buffer;
};

struct string_builder *new_string_builder() {
  struct string_builder *s = malloc(sizeof(*s));
  s->length = 0;
  s->buffer_size = DEFAULT_SIZE;
  s->buffer = malloc(sizeof(*(s->buffer)) * DEFAULT_SIZE);
  return s;
}

void sb_push_char(struct string_builder *s, char c) {
  size_t length = ++(s->length);
  if (length >= s->buffer_size) {
    s->buffer_size *= 2;
    s->buffer = realloc(s->buffer, sizeof(*(s->buffer)) * s->buffer_size);
  }

  s->buffer[s->length - 1] = c;
}

char *sb_finalize(struct string_builder *s) {
  sb_push_char(s, '\0');
  char *buf = realloc(s->buffer, sizeof(*(s->buffer)) * s->length);
  free(s);

  return buf;
}
