/*
 * File:    Cubes.ino
 * Version: 1.0
 * Author:  Blake Kus (blakekus@gmail.com)
 * License: GPLv3
 */

#include "SPI.h"
#include "Cube.h"

#define DELAY 1500
#define CUBE_SIZE 4

Cube cube;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
}

void loop(void) {
  // Solid
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, RED);
  delay(DELAY);
  // Walls only
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, GREEN, 1);
  delay(DELAY);
  // Edges only
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, BLUE, 2);
  delay(DELAY);
  // Walls and inner different colours
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, WHITE, 3, RED);
  delay(DELAY);
  // Edges and inner different colours
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, PURPLE, 4, WHITE);
  delay(DELAY);
  // Sphere
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, BLACK, 4, RED);
  delay(DELAY);
  // Hollow sphere
  cube.all(BLACK);
  cube.box(0, 0, 0, CUBE_SIZE-1, CUBE_SIZE-1, CUBE_SIZE-1, BLACK, 4, RED);
  cube.box(1, 1, 1, CUBE_SIZE-2, CUBE_SIZE-2, CUBE_SIZE-2, BLACK);
  delay(DELAY);
}
