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
 * - Make parser understand colour aliases, such as RED
 * - Set global cursor position when writing to a location
 * - Decide how to represent the hidden location. Using -1 for now
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"
#include "engine.h"
#include "parser.h"

byte parseCommand(
  char *message, byte length, byte *position, command_t **command
);

byte parseRGB(char *message, byte length, byte *position, rgb_t *rgb);
byte parsePosition(char *message, byte length, byte *position, byte *positionX, byte *positionY, byte *positionZ);
byte parseOffset(char *message, byte length, byte *position, byte *offset);
byte parseAxis(char *message, byte length, byte *position, byte *axis);

byte checkForHexadecimal(char *message, byte length, byte *position, byte *digit);
byte checkForOffset(char *message, byte length, byte *position, byte *digit);
byte checkForAxis(char *message, byte length, byte *position, byte *digit);

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

byte parseCommandAll(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);

  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeAll(bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandSet(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte positionX;
  byte positionY;
  byte positionZ;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parsePosition(message, length, position, & positionX, & positionY, & positionZ);
  skipWhitespace(message, length, position);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeSet( positionX, positionY, positionZ, bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandNext(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeNext(bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandCopyplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte distance;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseAxis(message, length, position, & axis);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & offset);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & distance);

  if (errorCode == 0) cubeCopyplane(axis, offset, distance);

  return(errorCode);
};

byte parseCommandSetplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseAxis(message, length, position, & axis);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & offset);
  skipWhitespace(message, length, position);
  errorCode = parseRGB(message, length, position, & bytecode->u.lit.colorFrom);

  if (errorCode == 0) cubeSetplane(axis, offset, bytecode->u.lit.colorFrom);

  return(errorCode);
};

byte parseCommandHelp(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte errorCode = 0;
  bytecode->executer = command->executer;

  if (serial) {
    serial->println("  Available commands:");
    serial->println("all <colour>;                               (eg: 'all RED;', or 'all ff0000;')");
    serial->println("set <location> <colour>;                    (eg: 'set 112 GREEN;', or 'set 112 00ff00;')");
    serial->println("next <colour>;                              (eg: 'next BLUE;', or 'next 0000ff;')");
    serial->println("setplane <axis> <offset> <colour>;          (eg: 'setplane X 2 BLUE;', or 'setplane Y 1 00ff00;')");
    serial->println("copyplane <axis> <from offset> <to offset>; (eg: 'copyplane X 2 1;')");
    serial->println("line <location1> <location2> <colour>;      (eg: 'line 000 333 WHITE;', or 'line 000 333 ffffff;') INCOMPLETE");
    serial->println("move <axis> <offset> <distance>;            (eg: 'move Z 3;', or 'move X -1;')                     INCOMPLETE");
    serial->println("shift <axis> <direction>;                   (eg: 'shift X 1;', or 'shift Y -1;')                   INCOMPLETE");
    serial->println("  Please see www.freetronics.com/cube for more information");
  }

  return(errorCode);
};

byte parseRGB(
  char  *message,
  byte   length,
  byte  *position,
  rgb_t *rgb) {

  byte digit;
  byte number;
  byte errorCode = 7;

  // Temporary test of colour alias detection. Nasty, replace with a generalised parser
  if (message[*position] == 'R' || message[*position] == 'r')
  {
    *rgb = RED;
    (*position) ++;
    errorCode = 0;
    return(errorCode);
  }
  if (message[*position] == 'G' || message[*position] == 'g')
  {
    *rgb = GREEN;
    (*position) ++;
    errorCode = 0;
    return(errorCode);
  }
  if (message[*position] == 'B' || message[*position] == 'b')
  {
    *rgb = BLUE;
    (*position) ++;
    errorCode = 0;
    return(errorCode);
  }


  if (checkForHexadecimal(message, length, position, & digit)) {
    number = digit;
    (*position) ++;

    if (checkForHexadecimal(message, length, position, & digit)) {
      rgb->color[0] = number * 16 + digit;
      (*position) ++;

      if (checkForHexadecimal(message, length, position, & digit)) {
        number = digit;
        (*position) ++;

        if (checkForHexadecimal(message, length, position, & digit)) {
          rgb->color[1] = number * 16 + digit;
          (*position) ++;

          if (checkForHexadecimal(message, length, position, & digit)) {
            number = digit;
            (*position) ++;

            if (checkForHexadecimal(message, length, position, & digit)) {
              rgb->color[2] = number * 16 + digit;
              (*position) ++;
              errorCode = 0;
            }
          }
        }
      }
    }
  }

  return(errorCode);
};

byte parsePosition(
  char  *message,
  byte   length,
  byte  *position,
  byte  *positionX,
  byte  *positionY,
  byte  *positionZ) {

  byte digit;
  byte errorCode = 6;

  if (checkForOffset(message, length, position, & digit)) {
    *positionX = digit;
    (*position) ++;

    if (checkForOffset(message, length, position, & digit)) {
      *positionY = digit;
      (*position) ++;

      if (checkForOffset(message, length, position, & digit)) {
        *positionZ = digit;
        (*position) ++;
        errorCode = 0;
      }
    }
  }

  return(errorCode);
};

byte parseAxis(
  char  *message,
  byte   length,
  byte  *position,
  byte  *axis) {

  byte digit;
  byte errorCode = 10;

  if (checkForAxis(message, length, position, & digit)) {
    *axis = digit;
    (*position) ++;
    errorCode = 0;
  }

  return(errorCode);
};

byte parseOffset(
  char  *message,
  byte   length,
  byte  *position,
  byte  *offset) {

  byte digit;
  byte errorCode = 6;

  if (checkForOffset(message, length, position, & digit)) {
    *offset = digit;
    (*position) ++;
    errorCode = 0;
  }

  return(errorCode);
};

byte checkForHexadecimal(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] >= '0'  &&  message[*position] <= '9') {
      *digit = message[*position] - '0';
      match = 1;
    }

    if (message[*position] >= 'A'  &&  message[*position] <= 'F') {
      *digit = message[*position] - 'A' + 10;
      match = 1;
    }

    if (message[*position] >= 'a'  &&  message[*position] <= 'f') {
      *digit = message[*position] - 'a' + 10;
      match = 1;
    }
  }

  return(match);
}

byte checkForAxis(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] == 'X' || message[*position] == 'x') {
      *digit = X;
      match = 1;
    }
    if (message[*position] == 'Y' || message[*position] == 'y') {
      *digit = Y;
      match = 1;
    }
    if (message[*position] == 'Z' || message[*position] == 'z') {
      *digit = Z;
      match = 1;
    }
  }

  return(match);
}

byte checkForOffset(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] >= '0'  &&  message[*position] <= '9') {
      *digit = message[*position] - '0';
      match = 1;
    }

    if (message[*position] == 'H'  ||  message[*position] == 'h') {
      *digit = -1;
      match = 1;
    }
  }

  return(match);
}

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
