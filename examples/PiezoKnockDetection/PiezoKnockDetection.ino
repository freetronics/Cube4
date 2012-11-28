/*
 * File:    PiezoKnockDetection.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (@jonoxer)
 * License: GPLv3
 */

/*
 * Note: This example requires a Piezo sensor in parallel with a 1M resistor
 * connected to the cube, with one side connected to GND and the other to
 * analog input A4. A suitable Piezo module with the resistor already fitted
 * is www.freetronics.com/sound
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

int flashDuration = 500;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
  pinMode(A4, INPUT);
  cube.all(GREEN);
}

void loop(void) {
  int knockReading = analogRead(A4);
  if(knockReading > 5)
  {
    cube.all(RED);
    delay(flashDuration);
  }
  cube.all(GREEN);
}
