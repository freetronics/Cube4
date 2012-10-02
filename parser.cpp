/*
 * File:    parser.cpp
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * Cube message parser.
 *
 * ToDo
 * ~~~~
 * - None, yet.
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"
#include "engine.h"
#include "parser.h"

byte parseCommand(
  char *message, byte length, byte *position, command_t **command
);
void skipToken(char *message, byte length, byte *position);
void skipWhitespace(char *message, byte length, byte *position);
boolean stringCompare(char *source, char *target);
boolean stringDelimiter(char character);

byte parser(
  char       *message,
  byte        length,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  byte position = 0;

  skipWhitespace(message, length, & position);

  command_t *command;

  errorCode = parseCommand(message, length, & position, & command);

  if (errorCode == 0) {
    skipWhitespace(message, length, & position);

    errorCode =
      (command->parser)(message, length, & position, command, bytecode);

    if (errorCode == 0) {
      skipWhitespace(message, length, & position);
    }
  }

  return(errorCode);
}

byte parseCommand(
  char       *message,
  byte        length,
  byte       *position,
  command_t **command) {

  byte errorCode = 5;

  for (byte commandType = 0;  commandType < commandCount;  commandType ++) {
    *command = & commands[commandType];

    if (stringCompare((*command)->name, & message[*position])) {
      skipToken(message, length, position);
      errorCode = 0;
      break;
    }
  }

  return(errorCode);
}

byte parseCommandHelp(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  if (serial) serial->println("No 'help' available yet");

  return(errorCode);
};

void skipToken(
  char *message,
  byte  length,
  byte *position) {

  while (*position < length) {
    if (stringDelimiter(message[*position])) break;
    (*position) ++;
  }
}

void skipWhitespace(
  char *message,
  byte  length,
  byte *position) {

  while (*position < length  &&  message[*position] == SPACE) (*position) ++;
}

boolean stringCompare(
  char *source,
  char *target) {

  byte index = 0;
  byte match = 1;

  while (stringDelimiter(source[index]) == 0  ||
         stringDelimiter(target[index]) == 0) {

    if (source[index] != target[index]) {
      match = 0;
      break;
    }
    index ++;
  }

  return(match);
}

boolean stringDelimiter(
  char character) {

  return(character == NUL  ||  character == SPACE  ||  character == RBRAC);
}
#endif
