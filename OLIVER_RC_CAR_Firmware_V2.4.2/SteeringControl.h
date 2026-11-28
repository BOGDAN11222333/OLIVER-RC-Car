#ifndef STEERING_CONTROL_H
#define STEERING_CONTROL_H

#include <Arduino.h>


// --------------------------------------------------
// Steering Control
// --------------------------------------------------

void SteeringControl_begin();


// Process steering command
//
// Examples:
//   LEFT
//   CENTER
//   RIGHT
//
void SteeringControl_execute(
    const String &command,
    const String &data
);


#endif