#pragma once

/** Represents a string-builder,
  which allows you to build a string
  by pushing characters onto the end of it. */
struct string_builder;

/** Creates a new string-builder */
struct string_builder *new_string_builder();

/** Pushes a character onto the end of a string-builder */
void sb_push_char(struct string_builder *, char);

/** Finalizes a string-builder, returning a
  null-terminated character buffer */
char *sb_finalize(struct string_builder *);
