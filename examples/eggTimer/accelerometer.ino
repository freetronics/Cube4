/*
 * File:    accelerometer.ino
 * Version: 0.0
 * Author:  Andy Gelme (@geekscape)
 * License: GPLv3
 */

/* ToDo
 * ~~~~
 * - None, yet.
 */

acceleration_t accelerations[10];

const static byte ACCELERATIONS = sizeof(accelerations) / sizeof(acceleration_t);

byte currentAcceleration = 0;
bool noShake = true;

acceleration_t noAcceleration = { 345, 380, 470 };
acceleration_t averageAcceleration = noAcceleration;

boolean actionAccelerometerInitialized = false;

void actionAccelerometerInitialize(void) {
  for (byte index = 0;  index < ACCELERATIONS;  index ++) {
    accelerations[index] = noAcceleration;
  }

  actionAccelerometerInitialized = true;
}

void actionAccelerometerHandler(void) {
  if (actionAccelerometerInitialized == false) actionAccelerometerInitialize();

  acceleration_t acceleration = {
    analogRead(PIN_ACCELERATION_X),
    analogRead(PIN_ACCELERATION_Y),
    analogRead(PIN_ACCELERATION_Z)
  };

  averageAcceleration.x += acceleration.x - accelerations[currentAcceleration].x;
  averageAcceleration.y += acceleration.y - accelerations[currentAcceleration].y;
  averageAcceleration.z += acceleration.z - accelerations[currentAcceleration].z;

  accelerations[currentAcceleration] = acceleration;

  currentAcceleration = (currentAcceleration + 1) % ACCELERATIONS;

  if (noShake) {
    if (averageAcceleration.x < 300  ||  averageAcceleration.x > 400) {
      noShake = false;
      Serial.println("shake: BEGIN");
      dumpAcceleration(averageAcceleration);
    }
  }
  else if (averageAcceleration.x >= 300  &&  averageAcceleration.x <= 400) {
    noShake = true;
    Serial.println("shake: END");
    dumpAcceleration(averageAcceleration);
  }
}

void dumpAcceleration(
  acceleration_t acceleration) {

  Serial.print("ax: "); Serial.println(acceleration.x);
  Serial.print("ay: "); Serial.println(acceleration.y);
  Serial.print("az: "); Serial.println(acceleration.z);
}
