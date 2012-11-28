/*
 * File:    Cube.h
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

#ifndef CUBE_h
#define CUBE_h

#include <avr/io.h>
#include <avr/interrupt.h>

#include <SPI.h>

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "color.h"
#include "engine.h"

#define RESOLUTION 65536

static const byte CUBE_SIZE = 4;
static const long TIMER1_PERIOD = 500;  // microseconds = 2 Khz per plane

// Analog pins ...

static const byte PIN_ACCELERATION_X = 0;
static const byte PIN_ACCELERATION_Y = 1;
static const byte PIN_ACCELERATION_Z = 2;

// Digital pins ...

// Chip: 74154.  RGB LED plane Z0 to Z3 high-side drivers.

static const byte PIN_LED_EN =  7;  // Enable is LOW
static const byte PIN_LED_A0 =  8;
static const byte PIN_LED_A1 =  9;
static const byte PIN_LED_A2 = 10;
static const byte PIN_LED_A3 = 11;

// Chip: MY9262.  RGB LED 16 bits per pixel APDM low-side drivers.

static const byte PIN_LED_LAT = 12;  // Enable is HIGH

static const byte COLOR_PLANE_RED   = 0;
static const byte COLOR_PLANE_GREEN = 1;
static const byte COLOR_PLANE_BLUE  = 2;

// Simple labels for each axis

static const byte X = 0;
static const byte Y = 1;
static const byte Z = 2;

class Cube {
  private:
    rgb_t currentColor;
    void fillPlaneZ(byte z, rgb_t rgb);
    void initializeTimer1(long period);
    void my9262WriteCommand(unsigned int command);
    void serialBegin(byte serialPort, long baudRate);

  public:
    Cube();
    void begin(byte serialPort = -1, long baudRate = 115200);
    boolean hasReceivedSerialCommand();
    void all(rgb_t rgb);
    void set(byte x, byte y, byte z, rgb_t rgb);
    void next(rgb_t rgb);
    void line(byte x1, byte y1, byte z1, byte x2, byte y2, byte z2, rgb_t rgb);
    void shift(byte axis, byte direction);
    void copyplane(byte axis, byte position, byte destination);
    void moveplane(byte axis, byte position, byte destination, rgb_t rgb);
    void setplane(byte axis, byte position, rgb_t rgb);
};

//extern long cubeTimer1Period;

extern rgb_t led[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];

extern Stream *serial;

extern void cubeAll(rgb_t rgb);
extern void cubeFillPlaneZ(byte z, rgb_t rgb);
extern void cubeSet( byte x, byte y, byte z, rgb_t rgb);
extern void cubeNext(rgb_t rgb);
extern void cubeLine(byte x1, byte y1, byte z1, byte x2, byte y2, byte z2, rgb_t rgb);
extern void cubeShift(byte axis, byte direction);
extern void cubeCopyplane(byte axis, byte position, byte destination);
extern void cubeMoveplane(byte axis, byte position, byte destination, rgb_t rgb_t);
extern void cubeSetplane(byte axis, byte position, rgb_t rgb);
extern byte parser(char *message, byte messageLength, bytecode_t *bytecode);
extern void serialHandler(void);
#endif
