/*
 * File:    engine.h
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

#ifndef ENGINE_h
#define ENGINE_h

typedef struct bytecode_s {
  byte (*executer)(struct bytecode_s bytecode);

  union {
    struct {
      byte step;
    } go;

    struct {
      rgb_t colorFrom;
      rgb_t colorTo;
      byte period;
    } lit;

    struct reset {};
    struct save {};
    struct stop {};
  }
    u;
}
  bytecode_t;  // 9 bytes

// bytecode_ sequence[50];  // 450 bytes

// byte sequenceCount = 0;
// byte sequenceLength = sizeof(sequence) / sizeof(bytecode_t);

// byte sequenceRunning = false;
// byte sequenceStep = 0;

byte executeNop(bytecode_t bytecode);
#endif
