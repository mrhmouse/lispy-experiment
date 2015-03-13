#pragma once
#define NEW(name) name = malloc(sizeof(*(name)))
#define RESIZE(name, count) name = realloc(name, sizeof(*(name)) * (count))
