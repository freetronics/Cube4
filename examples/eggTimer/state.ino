/*
 * File:    state.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - Pulse by separating the rectangle coordinates and color list.
 *   - Color list item should be referenced indirectly by a given rectangle.
 * - Smoother interpolation between pattern changes.
 * - Handle "states[state].nextTime" overflow.
 * - Nicer patterns !
 */

byte state = STATE_IDLE;

byte stateIdleCounter = 0;

void actionStateHandler(void) {
  long timeNow = millis();

  if (timeNow > states[state].nextTime) {
    (* states[state].handler)();

    states[state].nextTime = timeNow + states[state].frequency;
  }
}

void stateIdleHandler(void) {
// Cycle through rectangles

  rgb_t rgbSaved1 = idleRectangles[IDLE_RECTANGLES - 1].rgb;

  for (byte index = 0;  index < IDLE_RECTANGLES;  index ++) {
    drawRectangle(idleRectangles[index]);  // cube.drawRectangle(...);

    rgb_t rgbSaved2 = idleRectangles[index].rgb;
    idleRectangles[index].rgb = rgbSaved1;
    rgbSaved1 = rgbSaved2;
  }

// Cycle through colors

  stateIdleCounter = (stateIdleCounter + 1) % 80;

  if (stateIdleCounter > 71) {
    byte red   = idleRectangles[0].rgb.color[COLOR_PLANE_RED];
    byte green = idleRectangles[0].rgb.color[COLOR_PLANE_GREEN];
    byte blue  = idleRectangles[0].rgb.color[COLOR_PLANE_BLUE];

    idleRectangles[0].rgb.color[COLOR_PLANE_RED]   = green;
    idleRectangles[0].rgb.color[COLOR_PLANE_GREEN] = blue;
    idleRectangles[0].rgb.color[COLOR_PLANE_BLUE]  = red;
  }
}
