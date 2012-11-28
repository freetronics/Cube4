/*
 * File:    RandomPastels.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

byte xPos;
byte yPos;
byte zPos;
byte rr;
byte gg;
byte bb;

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {

  xPos = random(4);
  yPos = random(4);
  zPos = random(4);
  rr = random(255);
  gg = random(255);
  bb = random(255);
  
  
  cube.set(xPos, yPos, zPos, RGB(rr, gg, bb));
  delay(2);
}