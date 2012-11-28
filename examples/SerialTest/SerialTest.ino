/*
 * File:    SerialTest.ino
 * Version: 1.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

void setup(void) {
#if defined(ARDUINO) && ARDUINO >= 101
  while (Serial == false) ;                 // Wait for Leonardo USB serial port
#endif

  cube.begin(0, 115200);                    // Port: Serial, Baudrate: 115200
}

void loop(void) {
}
