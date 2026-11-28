#include <Arduino.h>

#include "VehicleControl.h"
#include "MotionControl.h"
#include "Logger.h"
#include "SteeringControl.h"
#include "Lights.h"


// --------------------------------------------------
// Hardware
// --------------------------------------------------



// --------------------------------------------------
// Initialization
// --------------------------------------------------

void VehicleControl_begin()
{
    
    // ------------------------------------------------
    // Motion Control
    // ------------------------------------------------

    MotionControl_begin();

    // ------------------------------------------------
    // Steering Control
    // ------------------------------------------------

    SteeringControl_begin();

    // ------------------------------------------------
    // LIGHTS
    // ------------------------------------------------

    Lights_begin();


    LOG_INFO(
        "VEHICLE",
        "Vehicle Control Ready"
    );
}


// --------------------------------------------------
// Command execution
// --------------------------------------------------

void VehicleControl_execute(
    const String &command,
    const String &data
)
{
    // ------------------------------------------------
    // LIGHTS
    // ------------------------------------------------

    if(command == "LIGHTS")
    {
    
    	Lights_execute(
            command,
            data
        );

    	return;
	}


    // ------------------------------------------------
    // DRIVE
    // ------------------------------------------------

    if(command == "DRIVE")
    {
        MotionControl_execute(data);

        return;
    }

    // ------------------------------------------------
    // STEER
    // ------------------------------------------------
    if(command == "STEER")
    {
        SteeringControl_execute(
            command,
            data
        );

        return;
    }
    // ------------------------------------------------
    // STEERING CALIBRATION
    // ------------------------------------------------

    if(command == "STEER_CAL")
    {
        SteeringControl_execute(
            command,
            data
        );

        return;
    }



    // ------------------------------------------------
    // Unknown command
    // ------------------------------------------------

    Serial.print(
        "[WARN][Vehicle] Unknown command: "
    );

    Serial.println(command);
}