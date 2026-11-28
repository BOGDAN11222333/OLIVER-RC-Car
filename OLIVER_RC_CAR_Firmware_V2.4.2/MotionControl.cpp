#include <Arduino.h>

#include "MotionControl.h"


// --------------------------------------------------
// Initialization
// --------------------------------------------------

void MotionControl_begin()
{
    // ------------------------------------------------
    // PWM setup
    // 20 kHz, 8-bit resolution
    // ------------------------------------------------

    ledcAttach(
        RPWM,
        20000,
        8
    );

    ledcAttach(
        LPWM,
        20000,
        8
    );


    Serial.println(
        "[Motion] PWM initialized"
    );

    Serial.println(
        "[Motion] Forward  = GPIO 15"
    );

    Serial.println(
        "[Motion] Reverse  = GPIO 16"
    );


    // ------------------------------------------------
    // Safety: outputs OFF at startup
    // ------------------------------------------------

    MotionControl_stop();
}


// --------------------------------------------------
// Set speed / direction
//
// speed:
//   +100 = full forward
//     0  = stop
//   -100 = full reverse
// --------------------------------------------------

void MotionControl_setSpeed(
    int speed
)
{
    // ------------------------------------------------
    // Limit command
    // ------------------------------------------------

    speed = constrain(
        speed,
        -100,
        100
    );


    // ------------------------------------------------
    // Convert percentage to PWM
    // 0...100 → 0...255
    // ------------------------------------------------

    int pwm = map(
        abs(speed),
        0,
        100,
        0,
        255
    );


    // ------------------------------------------------
    // Debug
    // ------------------------------------------------

    Serial.print(
        "[Motion] Speed = "
    );

    Serial.print(speed);

    Serial.print(
        " | PWM = "
    );

    Serial.println(pwm);


    // ------------------------------------------------
    // Forward
    // ------------------------------------------------

    if(speed > 0)
    {
        ledcWrite(
            RPWM,
            pwm
        );

        ledcWrite(
            LPWM,
            0
        );

        return;
    }


    // ------------------------------------------------
    // Reverse
    // ------------------------------------------------

    if(speed < 0)
    {
        ledcWrite(
            RPWM,
            0
        );

        ledcWrite(
            LPWM,
            pwm
        );

        return;
    }


    // ------------------------------------------------
    // Stop
    // ------------------------------------------------

    MotionControl_stop();
}


void MotionControl_execute(
    const String &data
)
{
    int speed = data.toInt();

    MotionControl_setSpeed(speed);
}

// --------------------------------------------------
// Stop
// --------------------------------------------------

void MotionControl_stop()
{
    ledcWrite(
        RPWM,
        0
    );

    ledcWrite(
        LPWM,
        0
    );

    Serial.println(
        "[Motion] STOP"
    );
}