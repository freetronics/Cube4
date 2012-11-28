/*
 * File:    AccelerometerColour.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

/*
 * Note: this sketch requires an analog accelerometer to be connected to the Cube,
 * with the Z-axis output connected to analog input A2 on the Cube. A suitable
 * accelerometer is the AM3X: www.freetronics.com/am3x
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
  pinMode(A2, INPUT);
}

void loop(void) {
  int zReading = analogRead(A2);
  if(zReading > 300)
  {
    cube.all(BLUE);
  }
  if(zReading < 200)
  {
    cube.all(GREEN);
  }
}
