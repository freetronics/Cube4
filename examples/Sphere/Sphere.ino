/*
 * File:    Sphere.ino
 * Version: 1.0
 * Author:  Blake Kus (blakekus@gmail.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

#define DELAY 1500

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  for (byte i = 1; i <= 4; i++) {
    // Hollow sphere
    cube.all(BLACK);
    cube.sphere(1, 1, 1, i, GREEN);
    delay(DELAY);
    // Sphere
    cube.all(BLACK);
    cube.sphere(1, 1, 1, i, RED, BLUE);
    delay(DELAY);
  }
}
