/*
 * File:    CubeInvaders.ino
 * Version: 1.0
 * Author:  Jonathan Oxer (jon@freetronics.com)
 * License: GPLv3
 */

/* 
 * Note: This sketch requires a Wii Nunchuck to be connected to the Cube as follows:
 * Yellow wire: SCL
 * Green wire: SDA
 * White wire: GND
 * Red wire: +5V
 *
 * Joystick moves the player. Z button fires. The cube flashes green with a successful
 * hit. There is no scoring, and no way to lose.
 */

#include "SPI.h"
#include "Cube.h"

Cube cube;

#include "Wire.h"
#include "WiiChuck.h"

WiiChuck wiichuck = WiiChuck();

unsigned int  joystickReadInterval  = 150;
unsigned long lastJoystickUpdate    = 0;
unsigned long lastEnemyMovement     = 0;
unsigned int  enemyMovementInterval = 600;

int playerX = 1;
int playerY = 0;
int playerZ = 1;

int enemyX;
int enemyY;
int enemyZ;

byte lastButtonZState = 0;

void setup(void) {
  // Serial port options for control of the Cube using serial commands are:
  // 0: Control via the USB connector (most common).
  // 1: Control via the RXD and TXD pins on the main board.
  // -1: Don't attach any serial port to interact with the Cube.
  cube.begin(0, 115200); // Start on serial port 0 (USB) at 115200 baud
  wiichuck.begin();
  wiichuck.update();
  enemyX = random(0, 4);
  enemyZ = random(0, 4);
  enemyY = 3;
}

void loop(void) {
  delay(20);
  wiichuck.update();
  cube.all(BLACK);
  checkTrigger();
  updatePlayerPosition();
  updateEnemyPosition();
  cube.set(playerX, playerY, playerZ, BLUE);
  cube.set(enemyX, enemyY, enemyZ, RED);
}

void updatePlayerPosition()
{
  if((millis() - lastJoystickUpdate) > joystickReadInterval)
  {
    int joyX;
    int joyY;

    joyX = wiichuck.readJoyX();
    joyY = wiichuck.readJoyY();

    if(joyX > 50)
      playerX++;

    if(joyX < -50)
      playerX--;

    if(joyY > 50)
      playerZ++;

    if(joyY < -50)
      playerZ--;

    if(playerX > 3)
      playerX = 0;

    if(playerX < 0)
      playerX = 3;

    if(playerZ > 3)
      playerZ = 0;

    if(playerZ < 0)
      playerZ = 3;

    lastJoystickUpdate = millis();
  }
}

void updateEnemyPosition()
{
  if((millis() - lastEnemyMovement) > enemyMovementInterval)
  {
    enemyY--;

    if(enemyY < 0)
    {
      enemyX = random(0, 4);
      enemyZ = random(0, 4);
      enemyY = 3;
    }
      
    lastEnemyMovement = millis();
  }
}

void checkTrigger()
{
  if((wiichuck.buttonZ) && (lastButtonZState == 0))
  {
    shoot();
  }
  if(wiichuck.buttonZ)
  {
    lastButtonZState = 1;
  } else {
    lastButtonZState = 0;
  } 
}

void shoot()
{
  cube.set(playerX, 1, playerZ, RGB(0xff, 0xff, 0x00));
  cube.set(playerX, 2, playerZ, RGB(0xff, 0xff, 0x00));
  cube.set(playerX, 3, playerZ, RGB(0xff, 0xff, 0x00));
  if((enemyX == playerX) && (enemyZ == playerZ))
    enemyY = -1;
    
  delay(100);
  
  if((enemyX == playerX) && (enemyZ == playerZ))
  {
    cube.all(GREEN);
    delay(200);
    cube.all(BLACK);
  }
}
