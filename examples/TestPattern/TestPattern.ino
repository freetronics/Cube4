/*
 * File:    TestPattern.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

byte testState = 0;
byte previousTestState = 0;

/**
 */
void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

/**
 */
void loop(void) {
  if(!cube.hasReceivedSerialCommand())
  {
    previousTestState = testState;
    switch(testState)
    {
      case 0:
        stepThroughLEDs();
        testState = 1;
        break;
      case 1:
        cycleCubeColours();
        testState = 2;
        break;
      case 2:
        fadeWhite();
        testState = 3;
        break;
      case 3:
        fadeWhite();
        testState = 4;
        break;
      case 4:
        fadeWhite();
        testState = 5;
        break;
      case 5:
        randomPastel();
        break;
    }
  } else {
    testState = 99;
  }
  
  if(testState == 99 && previousTestState != 99)
  {
    cube.all(BLACK);
    previousTestState = 99;
  }
}

void cycleCubeColours()
{
  cube.all(RED);
  delay(1000);
  cube.all(GREEN);
  delay(1000);
  cube.all(BLUE);
  delay(1000);
  cube.all(ORANGE);
  delay(1000);
  cube.all(PINK);
  delay(1000);
  cube.all(RGB(0x00, 0x44, 0xA));
  delay(1000);
}

void stepThroughLEDs()
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
        cube.all(BLACK);
        cube.set(xPos, yPos, zPos, WHITE);
        xPos++;
        i++;
        delay(500);
      }
      yPos--;
    }
    zPos++;
  }
}

void cyclePlaneColours()
{
  for (byte i = 0; i < 4; i++)
  {
    cube.all(BLACK);
    cube.setplane(Z, i, RED);
    delay(1000);
    cube.all(BLACK);
    cube.setplane(Z, i, GREEN);
    delay(1000);
    cube.all(BLACK);
    cube.setplane(Z, i, BLUE);
    delay(1000);
  }
}

void fadeWhite()
{
  for (byte i = 0; i < 255; i++)
  {
    cube.all(RGB(i, i, i));
    delay(3);
  }
  for (byte i = 255; i > 0; i--)
  {
    cube.all(RGB(i, i, i));
    delay(3);
  }
}

void randomPastel()
{  
  cube.set(random(4), random(4), random(4), RGB(random(255), random(255), random(255)));
  delay(2);
}
