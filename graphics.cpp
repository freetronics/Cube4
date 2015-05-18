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

void Swap(
  byte &a,
  byte &b) {
  byte c = a;
  a = b;
  b = c;
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

// http://en.wikipedia.org/wiki/Bresenham's_line_algorithm
void cubeLine(
  byte x1,
  byte y1,
  byte z1,
  byte x2,
  byte y2,
  byte z2,
  rgb_t rgb) {

  byte x;
  byte delta_x;
  byte step_x;
  byte y;
  byte delta_y;
  byte step_y;
  byte z;
  byte delta_z;
  byte step_z;
  boolean swap_xy;
  boolean swap_xz;
  float drift_xy;
  float drift_xz;
  byte cx;
  byte cy;
  byte cz;
  
  // 'steep' xy Line, make longest delta x plane  
  swap_xy = abs(y2 - y1) > abs(x2 - x1);
  if (swap_xy) {
      Swap(x1, y1);
      Swap(x2, y2);
  }
              
  // do same for xz
  swap_xz = abs(z2 - z1) > abs(x2 - x1);
  if (swap_xz) {
      Swap(x1, z1);
      Swap(x2, z2);
  }
  
  // delta is Length in each plane
  delta_x = abs(x2 - x1);
  delta_y = abs(y2 - y1);
  delta_z = abs(z2 - z1);
  
  // drift controls when to step in 'shallow' planes
  // starting value keeps Line centred
  drift_xy = (delta_x / 2);
  drift_xz = (delta_x / 2);
  
  // direction of line
  step_x = 1;
  if (x1 > x2) {
    step_x = -1;
  }
  step_y = 1;
  if (y1 > y2) {
    step_y = -1;
  }
  step_z = 1;
  if (z1 > z2) {
    step_z = -1;
  }
  
  // starting point
  y = y1;
  z = z1;
  
  // step through longest delta (which we have swapped to x)
  for (x = x1; x != x2 + step_x; x += step_x) {
      
      // copy position
      cx = x;
      cy = y;
      cz = z;

      // unswap (in reverse)
      if (swap_xz) {
        Swap(cx, cz);
      }
      if (swap_xy) {
        Swap(cx, cy);
      }

      // passes through this point
      cubeSet(cx, cy, cz, rgb);
      
      // update progress in other planes
      drift_xy = drift_xy - delta_y;
      drift_xz = drift_xz - delta_z;

      // step in y plane
      if (drift_xy < 0) {
          y = y + step_y;
          drift_xy = drift_xy + delta_x;
      }  
      
      // same in z
      if (drift_xz < 0) {
          z = z + step_z;
          drift_xz = drift_xz + delta_x;
      }
  }
}

void Cube::box(
  byte x1,
  byte y1,
  byte z1,
  byte x2,
  byte y2,
  byte z2,
  rgb_t rgb,
  byte style,
  rgb_t fill) {

  cubeBox(x1, y1, z1, x2, y2, z2, rgb, style, fill);
}

void cubeBox(
  byte x1,
  byte y1,
  byte z1,
  byte x2,
  byte y2,
  byte z2,
  rgb_t rgb,
  byte style,
  rgb_t fill) {

  boolean edge;
  boolean face;

  for (byte x = x1;  x <= x2;  x++) {
    for (byte y = y1;  y <= y2;  y++) {
      for (byte z = z1;  z <= z2;  z++) {
        face = false;
        edge = false;
        if (x == x1 || x == x2 || y == y1 || y == y2 || z == z1 || z == z2) {
          face = true;
          if (((x == x1 || x == x2) && (y == y1 || y == y2)) || ((x == x1 || x == x2) && (z == z1 || z == z2)) || ((y == y1 || y == y2) && (z == z1 || z == z2))) {
            edge = true;
          }
        }
        if (style == 0) {
          // solid
          cubeSet(x, y, z, rgb);
        } else if (style == 1) {
          // walls only
          if (face) {
            cubeSet(x, y, z, rgb);
          }
        } else if (style == 2) {
          // edges
          if (edge) {
            cubeSet(x, y, z, rgb);
          }
        } else if (style == 3) {
          // walls and inner different colours
          if (face) {
            cubeSet(x, y, z, rgb);
          } else {
            cubeSet(x, y, z, fill);
          }
        } else if (style == 4) {
          // edges and inner different colours
          if (edge) {
            cubeSet(x, y, z, rgb);
          } else {
            cubeSet(x, y, z, fill);
          }
        }
      }
    }
  }
}

void Cube::sphere(
  byte x1,
  byte y1,
  byte z1,
  byte size,
  rgb_t rgb,
  rgb_t fill) {

  cubeSphere(x1, y1, z1, size, rgb, fill);
}

void cubeSphere(
  byte x1,
  byte y1,
  byte z1,
  byte size,
  rgb_t rgb,
  rgb_t fill) {

  byte subtract;
  byte add;
  byte x2;
  byte y2;
  byte z2;

  // Dodgy, but it works because we are only dealing with 4x4x4
  if (size <= 2) {
    add = size - 1;
    x2 = x1 + add;
    y2 = y1 + add;
    z2 = z1 + add;
    cubeBox(x1, y1, z1, x2, y2, z2, rgb);
  } else {
    if (size == 3) {
      subtract = 1;
      add = 1;
    } else {
      subtract = 1;
      add = 2;
    }
    x2 = x1 + add;
    y2 = y1 + add;
    z2 = z1 + add;
    x1 -= subtract;
    y1 -= subtract;
    z1 -= subtract;
    cubeBox(x1, y1, z1, x2, y2, z2, BLACK, 4, rgb);
    cubeBox(x1+1, y1+1, z1+1, x2-1, y2-1, z2-1, fill);
  }
}

void Cube::shift(
  byte axis,
  byte direction) {

  cubeShift(axis, direction);
}

void cubeShift(
  byte axis,
  byte direction) {

  if( direction == '+' )
  {
    for (byte i = CUBE_SIZE - 1; i > 0; i--) {
      cubeCopyplane(axis, i - 1, i);
    }
    cubeSetplane(axis, 0, BLACK);
  }
  if( direction == '-' )
  {
    for (byte i = 0; i < CUBE_SIZE - 1; i++) {
      cubeCopyplane(axis, i + 1, i);
    }
    cubeSetplane(axis, 3, BLACK);
  }
}

void Cube::copyplane(
  byte axis,
  byte position,
  byte destination) {

  cubeCopyplane(axis, position, destination);
}

void cubeCopyplane(
  byte axis,
  byte position,
  byte destination) {

  if( axis == X)
  {

    byte y = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte y = 0;  y < CUBE_SIZE;  y++) {
        cubeSet(destination, y, z, RGB(led[position][y][z].color[COLOR_PLANE_RED], led[position][y][z].color[COLOR_PLANE_GREEN], led[position][y][z].color[COLOR_PLANE_BLUE]));
      }
    }
  }
  if( axis == Y)
  {
    byte x = 0;
    byte z = 0;
    for (byte z = 0;  z < CUBE_SIZE;  z++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, destination, z, RGB(led[x][position][z].color[COLOR_PLANE_RED], led[x][position][z].color[COLOR_PLANE_GREEN], led[x][position][z].color[COLOR_PLANE_BLUE]));
      }
    }
  }
  if( axis == Z)
  {
    byte x = 0;
    byte y = 0;
    for (byte y = 0;  y < CUBE_SIZE;  y++) {
      for (byte x = 0;  x < CUBE_SIZE;  x++) {
        cubeSet(x, y, destination, RGB(led[x][y][position].color[COLOR_PLANE_RED], led[x][y][position].color[COLOR_PLANE_GREEN], led[x][y][position].color[COLOR_PLANE_BLUE]));
      }
    }
  }
}

void Cube::moveplane(
  byte axis,
  byte position,
  byte destination,
  rgb_t rgb) {

  cubeMoveplane(axis, position, destination, rgb); 
}

void cubeMoveplane(
  byte axis,
  byte position,
  byte destination,
  rgb_t rgb) {

  cubeCopyplane(axis, position, destination);
  cubeSetplane(axis, position, rgb);
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
