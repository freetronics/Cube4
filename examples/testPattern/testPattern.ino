/*
 * File:    testPattern.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - Serial testing commands.
 */

#include <SPI.h>
#include "Cube.h"

Cube cube;

void setup(void) {
  Serial.begin(38400);

#if defined(ARDUINO) && ARDUINO >= 101
//while (Serial == false) ;                 // Wait for Leonardo USB serial port
#endif

  Serial.println("Cube: test pattern");
  Serial.println("~~~~~~~~~~~~~~~~~~");

  cube.begin();

  cube.all(RED);
//cube.all(RGB(0xff, 0x00, 0x00));      // RED

  cube.set(0, 0, 0, BLUE);
//cube.all();                           // current foreground color
}

void loop(void) {
}
