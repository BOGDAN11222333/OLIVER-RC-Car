#ifndef MOTION_CONTROL_H
#define MOTION_CONTROL_H

#include <Arduino.h>


// --------------------------------------------------
// PWM outputs
// --------------------------------------------------

#define RPWM 15
#define LPWM 16


// --------------------------------------------------
// Motion Control
// --------------------------------------------------

void MotionControl_begin();

void MotionControl_setSpeed(
    int speed
);

void MotionControl_execute(
    const String &data
);

void MotionControl_stop();


#endif