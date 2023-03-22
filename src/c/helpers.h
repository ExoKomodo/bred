#pragma once

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

int bred_dead(EditorConfig, const char*, int);
/*
  DOC: Returns the terminal back to the mode where characters are echoed back
*/
void _bred_raw_disable(EditorConfig);
/*
  DOC: Prevents the echoing of characters in the terminal when the user types.
  This allows us to fully manipulate the printing of characters given to bred.
*/
EditorConfig bred_raw_enable(EditorConfig);

int bred_dead(EditorConfig editor_config, const char* err, int code) {
  perror(err);
  bred_raw_disable(editor_config);
  exit(code);
  return code;
}

void bred_raw_disable(EditorConfig editor_config) {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, editor_config.original_terminal_data) == -1) {
    bred_dead(editor_config, "tcsetattr", 1);
  }
}

EditorConfig bred_raw_enable(EditorConfig editor_config) {
  if (tcgetattr(STDIN_FILENO, editor_config.original_terminal_data) == -1) {
    bred_dead(editor_config, "tcgetattr", 1);
  }

  editor_config.terminal_data = editor_config.original_terminal_data;
  tcgetattr(STDIN_FILENO, editor_config.terminal_data);
  // BRKINT: When BRKINT is turned on, a break condition will cause a SIGINT signal to be sent to the program, like pressing CTRL+c
  // ICRNL: Translate CTRL+m and enter key into carriage return
  // INPCK: Enables parity checking, which doesn’t seem to apply to modern terminal emulators
  /*
    ISTRIP: Causes the 8th bit of each input byte to be stripped, meaning it will set it to 0.
    This was probably already turned off
  */
  // IXON: Ignore CTRL+s suspend and CTRL+q resume
  editor_config.terminal_data->c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // OPOST: Turns off output processing that translates newlines to carriage returns
  editor_config.terminal_data->c_oflag &= ~(OPOST);
  /*
    CS8: not a flag, it is a bit mask with multiple bits,
    which we set using the bitwise-OR (|) operator unlike all the flags we are turning off.
    It sets the character size (CS) to 8 bits per byte.
  */
  editor_config.terminal_data->c_cflag |= (CS8);
  // ECHO: Ignore echo of characters
  // ICANON: Ignore enter key press requirement for input
  // IEXTEN: Ignore CTRL+v waiting for a character to type literally
  // ISIG: Ignore CTRL+c cancel and CTRL+z background
  editor_config.terminal_data->c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  editor_config.terminal_data->c_cc[VMIN] = 0;
  // Unit is 1/10 of a second
  editor_config.terminal_data->c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, editor_config.terminal_data) == -1) {
    bred_dead(editor_config, "tcsetattr", 1);
  }

  return editor_config;
}
