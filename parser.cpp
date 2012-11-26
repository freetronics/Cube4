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
byte parseDirection(char *message, byte length, byte *position, byte *direction);

byte checkForHexadecimal(char *message, byte length, byte *position, byte *digit);
byte checkForOffset(char *message, byte length, byte *position, byte *digit);
byte checkForAxis(char *message, byte length, byte *position, byte *digit);
byte checkForDirection(char *message, byte length, byte *position, byte *digit);

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

byte parseCommandShift(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte direction;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseAxis(message, length, position, & axis);
  skipWhitespace(message, length, position);
  errorCode = parseDirection(message, length, position, & direction);

  if (errorCode == 0) cubeShift(axis, direction);

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
  byte destination;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseAxis(message, length, position, & axis);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & offset);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & destination);

  if (errorCode == 0) cubeCopyplane(axis, offset, destination);

  return(errorCode);
};

byte parseCommandMoveplane(
  char       *message,
  byte        length,
  byte       *position,
  command_t  *command,
  bytecode_t *bytecode) {

  byte axis;
  byte offset;
  byte destination;
  rgb_t rgb;
  byte errorCode = 0;
  bytecode->executer = command->executer;

  skipWhitespace(message, length, position);
  errorCode = parseAxis(message, length, position, & axis);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & offset);
  skipWhitespace(message, length, position);
  errorCode = parseOffset(message, length, position, & destination);
  skipWhitespace(message, length, position);
  errorCode = parseRGB(message, length, position, & rgb);

  if (errorCode == 0) cubeMoveplane(axis, offset, destination, rgb);

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
    serial->println("  *** Available commands ***");
    serial->println("Entire cube:");
    serial->println("  all <colour>;                                        (eg: 'all RED;', or 'all ff0000;')");
    serial->println("  shift <axis> <direction>;                            (eg: 'shift X +;', or 'shift Y -;')");
    serial->println("Single LED:");
    serial->println("  set <location> <colour>;                             (eg: 'set 112 GREEN;', or 'set 112 00ff00;')");
    serial->println("  next <colour>;                                       (eg: 'next BLUE;', or 'next 0000ff;')");
    serial->println("One axis:");
    serial->println("  setplane <axis> <offset> <colour>;                   (eg: 'setplane X 2 BLUE;', or 'setplane Y 1 00ff00;')");
    serial->println("  copyplane <axis> <from offset> <to offset>;          (eg: 'copyplane X 2 1;')");
    serial->println("  moveplane <axis> <from offset> <to offset> <colour>; (eg: 'move Z 1 3 BLACK;', or 'move X 3 0 GREEN;')");
    //serial->println("  Graphics and shapes:");
    //serial->println("line <location1> <location2> <colour>;      (eg: 'line 000 333 WHITE;', or 'line 000 333 ffffff;') INCOMPLETE");
    serial->println("Supported colour aliases:");
    serial->println("  BLACK BLUE GREEN ORANGE PINK PURPLE RED WHITE YELLOW");
    serial->println("  *** Please see www.freetronics.com/cube for more information ***");
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
  if ((message[*position] == 'B' || message[*position] == 'b')
      && (message[*position + 1] == 'L' || message[*position + 1] == 'l')
      && (message[*position + 2] == 'A' || message[*position + 2] == 'a')
      && (message[*position + 3] == 'C' || message[*position + 3] == 'c')
      && (message[*position + 4] == 'K' || message[*position + 4] == 'k'))
  {
    *rgb = BLACK;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'B' || message[*position] == 'b')
      && (message[*position + 1] == 'L' || message[*position + 1] == 'l')
      && (message[*position + 2] == 'U' || message[*position + 2] == 'u')
      && (message[*position + 3] == 'E' || message[*position + 3] == 'e'))
  {
    *rgb = BLUE;
    (*position) +=4;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'G' || message[*position] == 'g')
      && (message[*position + 1] == 'R' || message[*position + 1] == 'r')
      && (message[*position + 2] == 'E' || message[*position + 2] == 'e')
      && (message[*position + 3] == 'E' || message[*position + 3] == 'e')
      && (message[*position + 4] == 'N' || message[*position + 4] == 'n'))
  {
    *rgb = GREEN;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'O' || message[*position] == 'o')
      && (message[*position + 1] == 'R' || message[*position + 1] == 'r')
      && (message[*position + 2] == 'A' || message[*position + 2] == 'a')
      && (message[*position + 3] == 'N' || message[*position + 3] == 'n')
      && (message[*position + 4] == 'G' || message[*position + 4] == 'g')
      && (message[*position + 5] == 'E' || message[*position + 5] == 'e'))
  {
    *rgb = ORANGE;
    (*position) += 6;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'P' || message[*position] == 'p')
      && (message[*position + 1] == 'I' || message[*position + 1] == 'i')
      && (message[*position + 2] == 'N' || message[*position + 2] == 'n')
      && (message[*position + 3] == 'K' || message[*position + 3] == 'k'))
  {
    *rgb = PINK;
    (*position) +=4;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'P' || message[*position] == 'p')
      && (message[*position + 1] == 'U' || message[*position + 1] == 'u')
      && (message[*position + 2] == 'R' || message[*position + 2] == 'r')
      && (message[*position + 3] == 'P' || message[*position + 3] == 'p')
      && (message[*position + 4] == 'L' || message[*position + 4] == 'l')
      && (message[*position + 5] == 'E' || message[*position + 5] == 'e'))
  {
    *rgb = PURPLE;
    (*position) += 6;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'R' || message[*position] == 'r')
      && (message[*position + 1] == 'E' || message[*position + 1] == 'e')
      && (message[*position + 2] == 'D' || message[*position + 2] == 'd'))
  {
    *rgb = RED;
    (*position) += 3;;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'W' || message[*position] == 'w')
      && (message[*position + 1] == 'H' || message[*position + 1] == 'h')
      && (message[*position + 2] == 'I' || message[*position + 2] == 'i')
      && (message[*position + 3] == 'T' || message[*position + 3] == 't')
      && (message[*position + 4] == 'E' || message[*position + 4] == 'e'))
  {
    *rgb = WHITE;
    (*position) += 5;
    errorCode = 0;
    return(errorCode);
  }

  if ((message[*position] == 'Y' || message[*position] == 'y')
      && (message[*position + 1] == 'E' || message[*position + 1] == 'e')
      && (message[*position + 2] == 'L' || message[*position + 2] == 'l')
      && (message[*position + 3] == 'L' || message[*position + 3] == 'l')
      && (message[*position + 4] == 'O' || message[*position + 4] == 'o')
      && (message[*position + 5] == 'W' || message[*position + 5] == 'w'))
  {
    *rgb = YELLOW;
    (*position) += 6;
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

byte parseDirection(
  char  *message,
  byte   length,
  byte  *position,
  byte  *direction) {

  byte digit;
  byte errorCode = 11;

  if (checkForDirection(message, length, position, & digit)) {
    *direction = digit;
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

byte checkForDirection(
  char *message,
  byte  length,
  byte *position,
  byte *digit) {

  byte match = 0;

  if (*position < length) {
    if (message[*position] == '+') {
      *digit = message[*position];
      match = 1;
    }

    if (message[*position] == '-') {
      *digit = message[*position];
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
