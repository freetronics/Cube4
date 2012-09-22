/*
 * File:    Cube.cpp
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * Low-level Cube drivers for 74154 and MY9262.
 *
 * ToDo
 * ~~~~
 * - Aim for 2 to 3 KHz scan rate rate (per color plane).
 * - loadColorPlaneZ() for 74154 set all 4 pins at once, using Port B directly.
 * - Rewrite SPI data transfer allowing pattern processing during data transfer.
 * - Test current gain control.
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"

byte currentColor  = COLOR_PLANE_RED;
byte currentPlaneZ = 0;

rgb_t led[CUBE_SIZE][CUBE_SIZE][CUBE_SIZE];  // [x][y][z].color[]

void loadColorPlaneZ(byte color, byte planeZ);

//long cubeLastTime = 0;
//long cubeTimer1Period = 0;

ISR(TIMER1_OVF_vect) {                             // 656 microseconds
//digitalWrite(7, digitalRead(7) ^ 1);             // Diagnosis via oscilloscope

  if (++ currentColor > COLOR_PLANE_BLUE) {
    currentColor  = COLOR_PLANE_RED;
    currentPlaneZ = (currentPlaneZ + 1) % CUBE_SIZE;
  }

  loadColorPlaneZ(currentColor, currentPlaneZ);

//long thisTime    = micros();
//cubeTimer1Period = thisTime - cubeLastTime;
//cubeLastTime     = thisTime;
}

Cube::Cube() {
}

void Cube::begin() {
//pinMode(7, OUTPUT);                              // Diagnosis via oscilloscope

// Assume these pins are all in consecutive order
  for (byte pin = PIN_LED_EN;  pin <= PIN_LED_LAT;  pin ++) {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
  }

  digitalWrite(PIN_LED_EN, HIGH);  // Disable 74154

  pinMode(MOSI, OUTPUT);
  pinMode(SCK,  OUTPUT);

// MY9262 Write Command
// --------------------
// 15:    Data format Mode0:        0
// 14-13: Reserved:                 0
// 12:    APDM mode:                0
// 11:    Counter reset:            0
// 10:    Synchronization = manual: 1  The default is "auto" = 0
//  9:    Global current HC:        1
//  8-4:  Global current data:      01011
//  3-2:  Reserved:                 00
//  1:    Sleep mode = normal:      0
//  0:    Reserved:                 0

//my9262WriteCommand(0x0d60);  // Change synchronization to be "manual"
  my9262WriteCommand(0x0ff0);  // Highest current
//my9262WriteCommand(0x0800);  // Lowest current

  all(BLACK);

  initializeTimer1(TIMER1_PERIOD);
}

void Cube::initializeTimer1(
  long period) {  // microseconds

  TCCR1A = 0;
  TCCR1B = _BV(WGM13);

  unsigned char clockSelectBits;
  long cycles = (F_CPU / 2000000) * period;

  if(cycles < RESOLUTION)              clockSelectBits = _BV(CS10);
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11);
  else if((cycles >>= 3) < RESOLUTION) clockSelectBits = _BV(CS11) | _BV(CS10);
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12);
  else if((cycles >>= 2) < RESOLUTION) clockSelectBits = _BV(CS12) | _BV(CS10);
  else        cycles = RESOLUTION - 1, clockSelectBits = _BV(CS12) | _BV(CS10);

  char oldSREG = SREG;
  cli();
  ICR1 = cycles;
  SREG = oldSREG;

  TIMSK1 = _BV(TOIE1);
  TCCR1B &= ~(_BV(CS10) | _BV(CS11) | _BV(CS12));
  TCCR1B |= clockSelectBits;
}

void Cube::my9262WriteCommand(
  unsigned int command) {

  digitalWrite(PIN_LED_LAT, LOW);

  for (byte b = 0;  b < 16;  b ++) {
    digitalWrite(MOSI, (command & 0x8000) == 0x8000);               // MSB first
    command <<= 1;

    if (b == 6) digitalWrite(PIN_LED_LAT, HIGH);   // MY9262 Write Command latch
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }

  digitalWrite(PIN_LED_LAT, LOW);
}

void loadColorPlaneZ(
  byte color,
  byte planeZ) {

  SPI.begin();
//SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  for (byte w = 0;  w < 15;  w ++) {
    byte value = led[w % CUBE_SIZE][w >> 2][planeZ].color[color];

    SPI.transfer(value);
    SPI.transfer(0x00);

    digitalWrite(PIN_LED_LAT, HIGH);  // MY9262 Data latch
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
    digitalWrite(PIN_LED_LAT, LOW);
  }

  SPI.end();

  byte value = led[3][3][planeZ].color[color];

  for (byte b = 0;  b < 8;  b ++) {  // LSB first
    digitalWrite(MOSI, (value & 0x80) == 0x80);
    value <<= 1;
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }

  digitalWrite(MOSI, LOW);

  for (byte b = 8;  b < 16;  b ++) {
    if (b == 14) digitalWrite(PIN_LED_LAT, HIGH);  // MY9262 Global latch
    digitalWrite(SCK, HIGH);
    digitalWrite(SCK, LOW);
  }

  digitalWrite(PIN_LED_EN, HIGH);  // Disable 74154
  digitalWrite(PIN_LED_LAT, LOW);  // MY9262 latch complete

  byte colorPlaneZSelect = color + (planeZ * 3);

  // Assume these pins are all in consecutive order
  for (byte pin = PIN_LED_A0;  pin <= PIN_LED_A3;  pin ++) {
    digitalWrite(pin, colorPlaneZSelect & 1);
    colorPlaneZSelect >>= 1;
  }

  digitalWrite(PIN_LED_EN, LOW);   // Enable 74154
}
#endif
