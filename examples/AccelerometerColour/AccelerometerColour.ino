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

#include "Cube.h"

Cube cube;

void setup(void) {
  cube.begin();
  pinMode(A2, INPUT);
  Serial.begin(38400);
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