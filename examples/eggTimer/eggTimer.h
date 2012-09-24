/*
 * File:    eggTimer.h
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - Improve idleRectangles sequence with more general drawing primitives.
 *   - Will be easier once the scripting engine is integrated.
 */

typedef struct {
  long nextTime;
  long frequency;
  void (*handler)(void);
}
  action_t;

void actionAccelerometerHandler(void);
void actionStateHandler(void);

action_t actions[] = {
  0, 100, actionAccelerometerHandler,
  0, 100, actionStateHandler
};

const static byte ACTIONS = sizeof(actions) / sizeof(action_t);

typedef struct {
  long x;
  long y;
  long z;
}
  acceleration_t;

const static byte STATE_IDLE      = 0;
const static byte STATE_SHAKE     = 1;
const static byte STATE_COUNTDOWN = 2;
const static byte STATE_ALARM     = 3;
const static byte STATE_ALARMED   = 4;
const static byte STATE_RESET     = 5;

void stateIdleHandler(void);
void stateShakeHandler(void);
void stateCountdownHandler(void);
void stateAlarmHandler(void);
void stateAlarmedHandler(void);
void stateResetHandler(void);

action_t states[] = {
  0, 100, stateIdleHandler
//0, 100, stateShakeHandler,
//0, 100, stateCountdownHandler,
//0, 100, stateAlarmHandler,
//0, 100, stateAlarmedHandler,
//0, 100, stateResetHandler
};

const static byte STATES = sizeof(states) / sizeof(action_t);

typedef struct {
  byte  x0; byte y0; byte z0;
  byte  x1; byte y1; byte z1;
  rgb_t rgb;
}
  rectangle_t;

rectangle_t idleRectangles[] = {
  { 1, 1, 0, 2, 2, 0, RGB(0x00, 0xf0, 0x40) },
  { 0, 0, 0, 3, 3, 0, RGB(0x00, 0xc0, 0x38) },
  { 1, 1, 1, 2, 2, 1, RGB(0x00, 0xa0, 0x30) },
  { 0, 0, 1, 3, 3, 1, RGB(0x00, 0x80, 0x28) },
  { 1, 1, 2, 2, 2, 2, RGB(0x00, 0x60, 0x20) },
  { 0, 0, 2, 3, 3, 2, RGB(0x00, 0x40, 0x18) },
  { 1, 1, 3, 2, 2, 3, RGB(0x00, 0x20, 0x10) },
  { 0, 0, 3, 3, 3, 3, RGB(0x00, 0x10, 0x08) }
};

const static byte IDLE_RECTANGLES = sizeof(idleRectangles)/sizeof(rectangle_t);
