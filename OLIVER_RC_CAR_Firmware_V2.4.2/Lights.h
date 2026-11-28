#ifndef LIGHTS_H
#define LIGHTS_H

#include <Arduino.h>

void Lights_begin();

void Lights_execute(
    const String &command,
    const String &data
);


bool Lights_isOn();


#endif