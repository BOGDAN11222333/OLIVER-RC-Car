#include "Lights.h"
#include "Logger.h"

static const uint8_t LIGHTS_PIN = 9;
static bool lightsState = false;


//--------------------------------------------------
// Initialize Lights
//--------------------------------------------------

void Lights_begin()
{
    pinMode(LIGHTS_PIN, OUTPUT);

    digitalWrite(
        LIGHTS_PIN,
        LOW
    );

    lightsState = false;

    LOG_INFO(
        "LIGHTS",
        "Lights Ready"
    );
}

void Lights_execute(
    const String &command,
    const String &data
)
{
    if(data == "ON")
    {
        digitalWrite(
            LIGHTS_PIN,
            HIGH
        );

        lightsState = true;

        LOG_INFO(
            "LIGHTS",
            "Lights ON"
        );

        return;
    }

    if(data == "OFF")
    {
        digitalWrite(
            LIGHTS_PIN,
            LOW
        );

        lightsState = false;

        LOG_INFO(
            "LIGHTS",
            "Lights OFF"
        );

        return;
    }

    LOG_WARN(
        "LIGHTS",
        "Invalid LIGHTS command: " + data
    );
}


//--------------------------------------------------
// Lights State
//--------------------------------------------------

bool Lights_isOn()
{
    return lightsState;
}