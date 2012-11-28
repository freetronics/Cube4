/*
 * File:    serial.cpp
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 *
 * Low-level Cube serial communications.
 *
 * ToDo
 * ~~~~
 * - None, yet.
 */

#ifndef CUBE_cpp
#define CUBE_cpp

#include "Cube.h"
#include "serial.h"

long    messageTimer = 0;
long    serialTimer = 0;
Stream *serial;

extern boolean readMessage(void);

void Cube::serialBegin(
  byte serialPort,
  long baudRate) {

  serial = NULL;

  switch (serialPort) {
    case 0:
      Serial.begin(baudRate);
      serial = & Serial;
      break;

    case 1:
      Serial1.begin(baudRate);
      serial = & Serial1;
      break;
  }

  if (serial) {
    serial->println("[Cube 1.0]");
    serial->println("Type 'help;'");
  }
}

void serialHandler(void) {
  if (serial) {
    long timeNow = millis();

    if (messageLength > 0) {
      if (timeNow >= messageTimer) messageLength = 0;
    }

    if (timeNow >= serialTimer) {
      serialTimer = timeNow + SERIAL_HANDLER_PERIOD;

      if (readMessage()) {
//      serial->println(message);
        receivedSerialCommand = true;
        bytecode_t bytecode = {};
        byte errorCode = parser(message, messageLength, & bytecode);
        messageLength = 0;
      }
    }
  }
}

boolean Cube::hasReceivedSerialCommand()
{
  return receivedSerialCommand;
}

boolean readMessage() {
  while (serial->available()) {
    messageTimer = millis() + MESSAGE_TIMEOUT;

    char data = serial->read();

    switch(data) {
      case CR:
      case SEMIC:
        return(true);  // complete message ready

      case HT:
      case LF:
        data = SPACE;

      case SPACE:
        if (messageLength < 1) break;
        if (message[messageLength - 1] == SPACE) break;

      default:
        if (data == LBRAC) messageLength = 0;

        if (messageLength < (sizeof(message) - 1)) {
          message[messageLength ++] = data;
          message[messageLength] = 0x00;
          if (data == RBRAC) return(true);  // complete message ready
        }
        break;
    }
  }

  return(false);  // partial message only
}
#endif
