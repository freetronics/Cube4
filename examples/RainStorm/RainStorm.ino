/*
 * File:    Rainstorm.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

#define LIGHTNINGINTERVAL 40 // How many raindrops between random lightning strikes, on average
#define RAINDROPDELAY 50     // Milliseconds between raindrop movements
#define MAXLIGHTNING 2       // Maximum number of flashes per lightning strike
#define SHOWCLOUDS 1         // Display a "cloud" layer at the top

byte drop1XPos;
byte drop1YPos;
byte drop1ZPos = 2;

byte drop2XPos;
byte drop2YPos;
byte drop2ZPos = 0;

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  if(drop1ZPos == 4)
  {
    drop1XPos = random(4);
    drop1YPos = random(4);
  }

  if(drop2ZPos == 4)
  {
    drop2XPos = random(4);
    drop2YPos = random(4);
  }

  cube.all(BLACK);
  clouds();

  if(drop1ZPos > 0)
  {
    drop1ZPos--;
    cube.set(drop1XPos, drop1YPos, drop1ZPos, BLUE);
  } else {
    drop1ZPos = 4;
  }

  if(drop2ZPos > 0)
  {
    drop2ZPos--;
    cube.set(drop2XPos, drop2YPos, drop2ZPos, BLUE);
  } else {
    drop2ZPos = 4;
  }

  delay(RAINDROPDELAY);

  lightning();
}

/**
 * Randomly display a lightning strike
 */
void lightning()
{
  if(random(LIGHTNINGINTERVAL) == 1)
  {
    int lightningFlashes = random(0, MAXLIGHTNING + 1);
    int i = 0;
    while (i < lightningFlashes)
    {
      cube.all(WHITE);
      delay(50);
      cube.all(BLACK);
      delay(150);
      i++;
    }
  }
}

/**
 * Display a white cloud layer at the top of the cube
 */
void clouds()
{
  if(SHOWCLOUDS)
  {
    cube.setplane(Z, 3, WHITE);
  }
}
