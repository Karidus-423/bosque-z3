#ifndef UTILS
#define UTILS

#include <cstddef>
#include <cstdio>
#include <cstdlib>
#define NULL_TERMINATOR 1

typedef struct File {
  char *stream;
  size_t length;
} File_t;

File_t *FileOpen(const char *filename);

#endif
