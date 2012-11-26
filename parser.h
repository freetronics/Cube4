/*
 * File:    parser.h
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

#ifndef PARSER_h
#define PARSER_h

static const byte NUL =   0x00;  // Null character
static const byte SPACE = 0x20;  // Space bar
static const byte RBRAC = 0x29;  // Right bracket ')'

typedef struct command_s {
  char  *name;
  byte (*parser) (
         char             *message,
         byte              length,
         byte             *position,
         struct command_s *command,
         bytecode_t       *bytecode);
  byte (*executer)(bytecode_t bytecode);
}
  command_t;

byte parseCommandAll(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandShift(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandSet(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandNext(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandSetplane(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandCopyplane(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandMoveplane(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);
byte parseCommandHelp(char *message, byte length, byte *position, command_t *command, bytecode_t *bytecode);

command_t commands[] = {
  "all",       parseCommandAll,       executeNop,
  "shift",     parseCommandShift,     executeNop,
  "set",       parseCommandSet,       executeNop,
  "next",      parseCommandNext,      executeNop,
  "setplane",  parseCommandSetplane,  executeNop,
  "copyplane", parseCommandCopyplane, executeNop,
  "moveplane", parseCommandMoveplane, executeNop,
  "help",      parseCommandHelp,      executeNop
};

byte commandCount = sizeof(commands) / sizeof(command_t);

/*
static const char *errorCodes[] = {
  "Success",                   // 0
  "Identifier isn't me",       // 1
  "Initial '(' expected",      // 2
  "Final ')' expected",        // 3
  "Invalid identifier list",   // 4
  "Invalid command",           // 5
  "Integer expected",          // 6
  "Colour (rrggbb) expected",  // 7
  "Invalid seq command",       // 8
  "Sequence memory full",      // 9
  "Axis designator expected",  // 10
  "Expected '+' or '-'"        // 11
};
 */
#endif
