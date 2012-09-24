/*
 * File:    graphics.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - None, yet.
 */

void drawBox(
  byte x0, byte y0, byte z0,
  byte x1, byte y1, byte z1,
  rgb_t    rgb) {

  for (byte z = z0;  z <= z1;  z ++) {
    for (byte y = y0;  y <= y1;  y ++) {
      for (byte x = x0;  x <= x1;  x ++) {
        cube.set(x, y, z, rgb);
      }
    }
  }
}

void drawRectangle(
  rectangle_t rectangle) {                    // Note: z0 == z1 for the moment !

  for (byte x = rectangle.x0;  x <= rectangle.x1;  x ++) {
    cube.set(x, rectangle.y0, rectangle.z0, rectangle.rgb);
  }

  for (byte y = rectangle.y0;  y <= rectangle.y1;  y ++) {
    cube.set(rectangle.x1, y, rectangle.z0, rectangle.rgb);
  }

  for (byte x = rectangle.x1;  x > rectangle.x0;  x --) {
    cube.set(x, rectangle.y1, rectangle.z0, rectangle.rgb);
  }

  for (byte y = rectangle.y1;  y > rectangle.y0;  y --) {
    cube.set(rectangle.x0, y, rectangle.z0, rectangle.rgb);
  }
}

void dumpXYZ(
  byte x, byte y, byte z) {

  Serial.print("x: "); Serial.println(x);
  Serial.print("y: "); Serial.println(y);
  Serial.print("z: "); Serial.println(z);
}
