#pragma once

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios _bred_original_termios;

/*
  DOC: Prevents the echoing of characters in the terminal when the user types.
  This allows us to fully manipulate the printing of characters given to bred.
*/
void bred_raw_enable();
/*
  DOC: Returns the terminal back to the mode where characters are echoed back
*/
void _bred_raw_disable();

void _bred_raw_disable() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &_bred_original_termios) == -1) {
    bred_dead("tcsetattr", 1);
  }
}

void bred_raw_enable() {
  if (tcgetattr(STDIN_FILENO, &_bred_original_termios) == -1) {
    bred_dead("tcgetattr", 1);
  }
  atexit(_bred_raw_disable);

  struct termios raw = _bred_original_termios;
  tcgetattr(STDIN_FILENO, &raw);
  // BRKINT: When BRKINT is turned on, a break condition will cause a SIGINT signal to be sent to the program, like pressing CTRL+c
  // ICRNL: Translate CTRL+m and enter key into carriage return
  // INPCK: Enables parity checking, which doesn’t seem to apply to modern terminal emulators
  /*
    ISTRIP: Causes the 8th bit of each input byte to be stripped, meaning it will set it to 0.
    This was probably already turned off
  */
  // IXON: Ignore CTRL+s suspend and CTRL+q resume
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  // OPOST: Turns off output processing that translates newlines to carriage returns
  raw.c_oflag &= ~(OPOST);
  /*
    CS8: not a flag, it is a bit mask with multiple bits,
    which we set using the bitwise-OR (|) operator unlike all the flags we are turning off.
    It sets the character size (CS) to 8 bits per byte.
  */
  raw.c_cflag |= (CS8);
  // ECHO: Ignore echo of characters
  // ICANON: Ignore enter key press requirement for input
  // IEXTEN: Ignore CTRL+v waiting for a character to type literally
  // ISIG: Ignore CTRL+c cancel and CTRL+z background
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
  raw.c_cc[VMIN] = 0;
  // Unit is 1/10 of a second
  raw.c_cc[VTIME] = 1;
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    bred_dead("tcsetattr", 1);
  }
}
