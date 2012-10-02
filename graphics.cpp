/*
 * File:    graphics.cpp
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * Higher-level Cube graphics.
 *
 * ToDo
 * ~~~~
 * - Check all parameter bounds !
 *
 * - Implement pattern scripting over serial line.
 * - Color gamma correction map for each LED intensity in Flash memory.
 *   - See post #37 http://www.microchip.com/forums/m479196-p2.aspx
 *   - See neuroelec.com/2011/04/led-brightness-to-your-eye-gamma-correction-no
 * - Scriptable sequence stored in EEPROM.
 * - Plugable sequence in third-party code.
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"

void Cube::all(
  rgb_t rgb) {

  for (byte z = 0;  z < CUBE_SIZE;  z ++) fillPlaneZ(z, rgb);
}

void Cube::fillPlaneZ(
  byte  z,
  rgb_t rgb) {

  for (byte y = 0;  y < CUBE_SIZE;  y++) {
    for (byte x = 0;  x < CUBE_SIZE;  x++) {
      set(x, y, z, rgb);
    }
  }
}

void Cube::set(
  byte  x,
  byte  y,
  byte  z,
  rgb_t rgb) {

  led[x][y][z].color[COLOR_PLANE_RED]   = rgb.color[COLOR_PLANE_RED];
  led[x][y][z].color[COLOR_PLANE_GREEN] = rgb.color[COLOR_PLANE_GREEN];
  led[x][y][z].color[COLOR_PLANE_BLUE]  = rgb.color[COLOR_PLANE_BLUE];
}
#endif
