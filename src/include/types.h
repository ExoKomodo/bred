#pragma once

#include <stdlib.h>

typedef struct termios termios;
struct EditorConfig {
  char* input;
  termios* terminal_data;
  size_t number_of_rows;
};
typedef struct EditorConfig EditorConfig;
int empty_editor_config(EditorConfig editor_config) {
  char* input = editor_config.input;
  free(input);
  return 0;
}
