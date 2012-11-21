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
#include "graphics.h"

void Cube::all(
  rgb_t rgb) {

  cubeAll(rgb);
}

void cubeAll(
  rgb_t rgb) {

  for (byte z = 0;  z < CUBE_SIZE;  z++) cubeFillPlaneZ(z, rgb);
}

void Cube::fillPlaneZ(
  byte  z,
  rgb_t rgb) {

  cubeFillPlaneZ(z, rgb);
}

void cubeFillPlaneZ(
  byte  z,
  rgb_t rgb) {

  for (byte y = 0;  y < CUBE_SIZE;  y++) {
    for (byte x = 0;  x < CUBE_SIZE;  x++) {
      cubeSet(x, y, z, rgb);
    }
  }
}

void Cube::set(
  byte  x,
  byte  y,
  byte  z,
  rgb_t rgb) {

  cubeSet(x, y, z, rgb);
}

void cubeSet(
  byte  x,
  byte  y,
  byte  z,
  rgb_t rgb) {

  led[x][y][z].color[COLOR_PLANE_RED]   = rgb.color[COLOR_PLANE_RED];
  led[x][y][z].color[COLOR_PLANE_GREEN] = rgb.color[COLOR_PLANE_GREEN];
  led[x][y][z].color[COLOR_PLANE_BLUE]  = rgb.color[COLOR_PLANE_BLUE];

  cursorX = x;
  cursorY = y;
  cursorZ = z;
}

void Cube::next(
  rgb_t rgb) {

  cubeNext(rgb);
}

void cubeNext(
  rgb_t rgb) {

  cursorX++;
  if(cursorX > CUBE_SIZE - 1)
  {
    cursorX = 0;
    cursorY++;
    if(cursorY > CUBE_SIZE - 1)
    {
      cursorY = 0;
      cursorZ++;
      if(cursorZ > CUBE_SIZE - 1)
      {
        cursorZ = 0;
      }
    }
  }
  
  cubeSet(cursorX, cursorY, cursorZ, rgb);
}

void Cube::line(
  byte x1,
  byte y1,
  byte z1,
  byte x2,
  byte y2,
  byte z2,
  rgb_t rgb) {

  cubeLine(x1, y1, z1, x2, y2, z2, rgb);
}

void cubeLine(
  byte x1,
  byte y1,
  byte z1,
  byte x2,
  byte y2,
  byte z2,
  rgb_t rgb) {

  //FIXME
}

void Cube::move(
  byte axis,
  byte offset,
  byte distance) {

  cubeMove(axis, offset, distance);
}

void cubeMove(
  byte axis,
  byte offset,
  byte distance) {

  //FIXME
}

void Cube::shift(
  byte axis,
  byte direction) {

  cubeShift(axis, direction);
}

void cubeShift(
  byte axis,
  byte direction) {

  //FIXME
}

void Cube::copyplane(
  byte axis,
  byte offset,
  byte distance) {

  cubeCopyplane(axis, offset, distance);
}

void cubeCopyplane(
  byte axis,
  byte offset,
  byte distance) {

  if( axis == X)
  {
    byte y = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte y = 0;  y < CUBE_SIZE;  y++) {
        cubeSet(offset + distance, y, z, RGB(led[offset][y][z].color[COLOR_PLANE_RED], led[offset][y][z].color[COLOR_PLANE_GREEN], led[offset][y][z].color[COLOR_PLANE_BLUE]));
      }
    }
  }
  if( axis == Y)
  {
    byte x = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, offset + distance, z, RGB(led[x][offset][z].color[COLOR_PLANE_RED], led[x][offset][z].color[COLOR_PLANE_GREEN], led[x][offset][z].color[COLOR_PLANE_BLUE]));
      }
    }
  }
  if( axis == Z)
  {
    byte x = 0;
    byte y = 0;
    for (byte y = 0;  y < CUBE_SIZE;  y++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, y, offset + distance, RGB(led[x][y][offset].color[COLOR_PLANE_RED], led[x][y][offset].color[COLOR_PLANE_GREEN], led[x][y][offset].color[COLOR_PLANE_BLUE]));
      }
    }
  }
}

void Cube::setplane(
  byte axis,
  byte offset,
  rgb_t rgb) {

  cubeSetplane(axis, offset, rgb);
}

void cubeSetplane(
  byte axis,
  byte offset,
  rgb_t rgb) {

  if( axis == X)
  {
    byte y = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte y = 0;  y < CUBE_SIZE;  y++) {
        cubeSet(offset, y, z, rgb);
      }
    }
  }

  if( axis == Y)
  {
    byte x = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, offset, z, rgb);
      }
    }
  }

  if( axis == Z)
  {
    byte x = 0;
    byte y = 0;
    for (byte y = 0;  y < CUBE_SIZE;  y++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, y, offset, rgb);
      }
    }
  }
}


#endif
