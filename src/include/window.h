#pragma once

#include <stdio.h>
#include <unistd.h>

BredVector bred_get_cursor_position() {
  BredVector result = {
    .x = 0,
    .y = 0,
  };
  char buffer[32];
  unsigned int i = 0;
  if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
    return result;
  }
  while (i < sizeof(buffer) - 1) {
    if (read(STDIN_FILENO, &buffer[i], 1) != 1) {
      break;
    }
    if (buffer[i] == 'R') {
      break;
    }
    i++;
  }
  buffer[i] = '\0';
  if (buffer[0] != '\x1b' || buffer[1] != '[') {
    return result;
  }
  int x;
  int y;
  if (sscanf(&buffer[2], "%d;%d", &y, &x) != 2) {
    return result;
  }
  result.x = x;
  result.y = y;
  return result;
}