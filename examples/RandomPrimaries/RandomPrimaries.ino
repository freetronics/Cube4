/*
 * File:    RandomPrimaries.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

byte xPos;
byte yPos;
byte zPos;

rgb_t colours[3] = {RED, GREEN, BLUE};

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {

  xPos = random(0, 4);
  yPos = random(0, 4);
  zPos = random(0, 4);
  byte i = random(0, 3);
  
  cube.set(xPos, yPos, zPos, colours[i]);
  delay(2);
}
