/*
 * File:    SunnyDay.ino
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
  cube.all(RGB(0x00, 0x00, 0x22));
  cube.setplane(Z, 0, GREEN);
  cube.set(0, 3, 3, RGB( 0xff, 0xff, 0x00));
  cube.set(0, 3, 2, RGB( 0x22, 0x22, 0x00));
  cube.set(0, 2, 3, RGB( 0x22, 0x22, 0x00));
  cube.set(0, 2, 2, RGB( 0x22, 0x22, 0x00));
  cube.set(1, 3, 3, RGB( 0x22, 0x22, 0x00));
  cube.set(1, 3, 2, RGB( 0x22, 0x22, 0x00));
  cube.set(1, 2, 3, RGB( 0x22, 0x22, 0x00));
  cube.set(1, 2, 2, RGB( 0x22, 0x22, 0x00));
}

void loop(void) {
}
