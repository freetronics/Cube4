/*
 * File:    eggTimer.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - Include freeRam() check.
 * - Handle "actions[index].nextTime" overflow.
 * - If USB serial port is connected, then disconnected, serial log stalls.
 *   - Check "Serial == false" in a logging function ?
 */

#include <SPI.h>
#include "Cube.h"
#include "eggTimer.h"

Cube cube;

void setup(void) {
  Serial.begin(38400);

#if defined(ARDUINO) && ARDUINO >= 101
//while (Serial == false) ;                 // Wait for Leonardo USB serial port
#endif

  cube.begin();
}

void loop(void) {
  long timeNow = millis();

  for (byte index = 0;  index < ACTIONS;  index ++) {
    if (timeNow > actions[index].nextTime) {
      (* actions[index].handler)();

      actions[index].nextTime = timeNow + actions[index].frequency;
    }
  }
}
