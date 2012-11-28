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
  cube.begin();
}

void loop(void) {

  xPos = random(0, 4);
  yPos = random(0, 4);
  zPos = random(0, 4);
  byte i = random(0, 3);
  
  cube.set(xPos, yPos, zPos, colours[i]);
  delay(2);
}
