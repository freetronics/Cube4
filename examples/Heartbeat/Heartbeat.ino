/*
 * File:    Heartbeat.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  rise();
  delay(40);
  fall();
  delay(100);
  rise();
  delay(40);
  fall();
  delay(1000);
}


/**
 * Illuminate one LED at a time, starting from the bottom
 */
void rise()
{
  int i = 0;
  int xPos = 0;
  int yPos = 0;
  int zPos = 0;

  while(zPos < 4)
  {
    yPos = 3;
    while(yPos > -1)
    {
      xPos = 0;
      while(xPos < 4)
      {
        cube.set(xPos, yPos, zPos, RED);
        xPos++;
        i++;
        delay(4);
      }
      yPos--;
    }
    zPos++;
  }
}

/**
 * Extinguish one LED at a time, starting from the top
 */
void fall()
{
  int xPos = 3;
  int yPos = 3;
  int zPos = 3;

  while(zPos > -1)
  {
    xPos = 3;
    while(xPos > -1)
    {
      yPos = 3;
      while(yPos > -1)
      {
        cube.set(xPos, yPos, zPos, BLACK);
        yPos--;
        delay(4);
      }
      xPos--;
    }
    zPos--;
  }
}
