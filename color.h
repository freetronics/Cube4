/*
 * File:    color.h
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

#ifndef COLOR_h
#define COLOR_h

typedef struct {
  byte color[3];
}
  rgb_t;

#define RGB(red, green, blue) (rgb_t) { red, green, blue }

#define BLACK  RGB(0x00, 0x00, 0x00)
#define BLUE   RGB(0x00, 0x00, 0xff)
#define GREEN  RGB(0x00, 0xff, 0x00)
#define ORANGE RGB(0xff, 0x45, 0x00)
#define PINK   RGB(0xff, 0x14, 0x44)
#define PURPLE RGB(0xff, 0x00, 0xff)
#define RED    RGB(0xff, 0x00, 0x00)
#define WHITE  RGB(0xff, 0xff, 0xff)
#define YELLOW RGB(0xff, 0xff, 0x00)

#endif
