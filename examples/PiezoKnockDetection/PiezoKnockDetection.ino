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

#include "Cube.h"

Cube cube;

int flashDuration = 500;

void setup(void) {
  cube.begin();
  pinMode(A4, INPUT);
  Serial.begin(38400);
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