/*
 * File:    serialTest.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - None, yet.
 */

#include <SPI.h>
#include "Cube.h"

Cube cube;

void setup(void) {
#if defined(ARDUINO) && ARDUINO >= 101
  while (Serial == false) ;                 // Wait for Leonardo USB serial port
#endif

  cube.begin(0, 38400);                     // Port: Serial, Baudrate: 38400
}

void loop(void) {
}
