/*
 * File:    RainStorm.ino
 * Version: 0.0
 * Author:  Jonathan Oxer (@jonoxer)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - Serial commands to change settings such as clouds and lightning
 * - Refactor raindrops as objects, and support varying numbers of drops
 */

#include <SPI.h>
#include "Cube.h"

#define LIGHTNINGINTERVAL 40  // How many raindrops between random lightning strikes, on average
#define RAINDROPDELAY     50  // Milliseconds between raindrop movements
#define MAXLIGHTNING       2  // Maximum number of flashes per lightning strike (0 for no lightning)
#define SHOWCLOUDS         1  // 1: Display a "cloud" layer at the top. 0: No clouds

byte drop1XPos;
byte drop1YPos;
byte drop1ZPos = 2;

byte drop2XPos;
byte drop2YPos;
byte drop2ZPos = 0;

Cube cube;

/**
 */
void setup(void) {
  cube.begin();
}

/**
 */
void loop(void) {
  cube.all(BLACK);
  drawClouds();
  drawRaindrops();
  drawLightning();
}

/**
 */
void drawRaindrops()
{
  updateRaindropPositions();
  cube.set(drop1XPos, drop1YPos, drop1ZPos, BLUE);
  cube.set(drop2XPos, drop2YPos, drop2ZPos, BLUE);
  delay(RAINDROPDELAY);
}

/**
 * Decrease raindrop altitude. Check if they have hit the ground, and create a new raindrop if they have.
 */
void updateRaindropPositions()
{
  if(drop1ZPos > 0)
  {
    drop1ZPos--;
  } else {
    drop1XPos = random(4);
    drop1YPos = random(4);
    drop1ZPos = 4;
  }

  if(drop2ZPos > 0)
  {
    drop2ZPos--;
  } else {
    drop2XPos = random(4);
    drop2YPos = random(4);
    drop2ZPos = 4;
  }
}

/**
 * Randomly display a lightning strike
 */
void drawLightning()
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
void drawClouds()
{
  if(SHOWCLOUDS)
  {
    int xPos = 0;
    int yPos = 0;
    while(xPos < 4)
    {
      yPos = 0;
      while(yPos < 4)
      {
        cube.set(xPos, yPos, 3, WHITE );
        yPos++;
      }
      xPos++;
    }
  }
}
