/*
 * File:    LaserShow.ino
 * Version: 1.0
 * Author:  Blake Kus (blakekus@gmail.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

#define DELAY 150

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  byte x = 0;
  byte y = 0;
  byte step_x = 1;
  byte step_y = 0;
  for (byte i = 0; i < 24; i++) {
    cube.all(BLACK);
    if (i <= 12) {
      cube.line(0, 3, 0, y, 3-x, 3, WHITE);
      cube.line(3, 3, 0, 3-x, 3-y, 3, BLUE);
      cube.line(3, 0, 0, 3-y, x, 3, GREEN);
      cube.line(0, 0, 0, x, y, 3, RED);
    } else {
      cube.line(0, 3, 3, y, 3-x, 0, WHITE);
      cube.line(3, 3, 3, 3-x, 3-y, 0, BLUE);
      cube.line(3, 0, 3, 3-y, x, 0, GREEN);
      cube.line(0, 0, 3, x, y, 0, RED);
    }
    if (x == 3 && y == 0) {
      step_x = 0;
      step_y = 1;
    } else if (x == 3 && y == 3) {
      step_x = -1;
      step_y = 0;
    } else if (x == 0 && y == 3) {
      step_x = 0;
      step_y = -1;
    } else if (x == 0 && y == 0) {
      step_x = 1;
      step_y = 0;
    }
    x += step_x;
    y += step_y;
    delay(DELAY);
  }
  cube.all(BLACK);
  x = 0;
  y = 0;
  step_x = 1;
  step_y = 0;
  for (byte i = 0; i < 16; i++) {
    cube.line(x, y, 0, x, y, 3, RED);
    if (x == 3 && y == 0) {
      step_x = 0;
      step_y = 1;
    } else if (x == 3 && y == 3) {
      step_x = -1;
      step_y = 0;
    } else if (x == 0 && y == 3) {
      step_x = 0;
      step_y = -1;
    } else if (x == 0 && y == 1) {
      step_x = 1;
      step_y = 0;
    } else if (x == 2 && y == 1) {
      step_x = 0;
      step_y = 1;
    } else if (x == 2 && y == 2) {
      step_x = -1;
      step_y = 0;
    }
    x += step_x;
    y += step_y;
    delay(DELAY);
  }
}
