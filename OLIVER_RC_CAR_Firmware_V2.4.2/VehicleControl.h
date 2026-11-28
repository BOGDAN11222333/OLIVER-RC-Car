#ifndef VEHICLE_CONTROL_H
#define VEHICLE_CONTROL_H

#include <Arduino.h>


// --------------------------------------------------
// Vehicle Control
// --------------------------------------------------

// Initialize all vehicle control modules
void VehicleControl_begin();


// Process a vehicle command
//
// Examples:
//   LIGHTS + ON
//   LIGHTS + OFF
//   DRIVE  + 50
//   DRIVE  + -50
//
void VehicleControl_execute(
    const String &command,
    const String &data
);


#endif