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

  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
}
