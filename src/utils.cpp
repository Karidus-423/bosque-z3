#include "utils.hpp"

File_t *FileOpen(const char *filename) {
  std::FILE *fp = std::fopen(filename, "r");
  if (fp == NULL) {
    printf("Error opening file\n");
    return NULL;
  }
  std::fseek(fp, 0, SEEK_END);
  size_t f_size = ftell(fp);
  rewind(fp);

  void *text = std::malloc(sizeof(char) * (f_size + NULL_TERMINATOR));

  if (std::fread(text, sizeof(char), f_size, fp) != f_size) {
    printf("Error reading file\n");
    free(text);
    fclose(fp);
    return NULL;
  }

  File_t *file = (File_t *)malloc(sizeof(File_t));
  file->stream = (char *)text;
  file->length = f_size;
  return file;
}
