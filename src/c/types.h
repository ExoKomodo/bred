#pragma once

typedef struct termios termios;
struct EditorConfig {
  char* input;
  termios* original_terminal_data;
  termios* terminal_data;
};
typedef struct EditorConfig EditorConfig;
