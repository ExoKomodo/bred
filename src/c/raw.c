#pragma once

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios _ded_original_termios;

/*
  DOC: Prevents the echoing of characters in the terminal when the user types.
  This allows us to fully manipulate the printing of characters given to ded.
*/
void ded_raw_enable();
/*
  DOC: Returns the terminal back to the mode where characters are echoed back
*/
void _ded_raw_disable();

void _ded_raw_disable() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &_ded_original_termios);
}

void ded_raw_enable() {
  tcgetattr(STDIN_FILENO, &_ded_original_termios);
  atexit(_ded_raw_disable);

  struct termios raw;
  tcgetattr(STDIN_FILENO, &raw);
  raw.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
