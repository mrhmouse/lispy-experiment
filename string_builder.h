#pragma once

struct string_builder;
struct string_builder *new_string_builder();

void sb_push_char(struct string_builder *, char);
char *sb_finalize(struct string_builder *);
