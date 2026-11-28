#include <Arduino.h>
#include <ESP32Servo.h>
#include <Preferences.h>
#include "SteeringControl.h"
#include "Logger.h"


// --------------------------------------------------
// Hardware
// --------------------------------------------------

#define SERVO_PIN 12


// --------------------------------------------------
// Servo
// --------------------------------------------------

static Servo steeringServo;
static Preferences steeringPreferences;

// --------------------------------------------------
// Steering calibration
// --------------------------------------------------

static int calibrationPosition = 90;
static int steeringZero = 90;

static int steeringAngle = 30;

static int steeringLeftLimit;
static int steeringRightLimit;

#define CALIBRATION_STEP       1
#define CALIBRATION_MIN        0
#define CALIBRATION_MAX        180


// --------------------------------------------------
// Initialization
// --------------------------------------------------

void SteeringControl_begin()
{
    steeringServo.setPeriodHertz(50);

    steeringServo.attach(
        SERVO_PIN,
        500,
        2400
    );

    // NEW ZERO POSITION
    steeringPreferences.begin(
        "steering",
        false
    );

    steeringZero = steeringPreferences.getInt(
        "zero",
        90
    );

    steeringAngle = steeringPreferences.getInt(
    "angle",
    30
    );

    steeringLeftLimit = constrain(
        steeringZero - steeringAngle,
        CALIBRATION_MIN,
        CALIBRATION_MAX
    );

    steeringRightLimit = constrain(
        steeringZero + steeringAngle,
        CALIBRATION_MIN,
        CALIBRATION_MAX
    );

    steeringPreferences.end();

    calibrationPosition = steeringZero;

    steeringServo.write(
        calibrationPosition
    );

    LOG_INFO(
        "STEERING",
        "Servo initialized on GPIO 12"
    );

    LOG_INFO(
        "STEERING",
        "Zero position = " +
        String(steeringZero)
    );
    LOG_INFO(
        "STEERING",
    "Steering angle = " +
        String(steeringAngle)
    );

    LOG_INFO(
        "STEERING",
        "Left limit = " +
        String(steeringLeftLimit)
    );

    LOG_INFO(
        "STEERING",
        "Right limit = " +
        String(steeringRightLimit)
    );

}
// --------------------------------------------------
// Steering command
// --------------------------------------------------

void SteeringControl_execute(
    const String &command,
    const String &data
)
{
    // ------------------------------------------------
    // STEERING CALIBRATION
    // ------------------------------------------------

    if(command == "STEER_CAL")
    {
        // --------------------------------------------
        // CALIBRATION - LEFT
        // --------------------------------------------

        if(data == "LEFT")
        {
            calibrationPosition -= CALIBRATION_STEP;

            calibrationPosition = constrain(
                calibrationPosition,
                CALIBRATION_MIN,
                CALIBRATION_MAX
            );

            steeringServo.write(
                calibrationPosition
            );

            LOG_INFO(
                "STEERING",
                "Calibration position = " +
                String(calibrationPosition)
            );

            return;
        }


        // --------------------------------------------
        // CALIBRATION - RIGHT
        // --------------------------------------------

        if(data == "RIGHT")
        {
            calibrationPosition += CALIBRATION_STEP;

            calibrationPosition = constrain(
                calibrationPosition,
                CALIBRATION_MIN,
                CALIBRATION_MAX
            );

            steeringServo.write(
                calibrationPosition
            );

            LOG_INFO(
                "STEERING",
                "Calibration position = " +
                String(calibrationPosition)
            );

            return;
        }


        // --------------------------------------------
        // CALIBRATION - ZERO
        // --------------------------------------------

        if(data == "ZERO")
        {
            steeringZero = calibrationPosition;

            LOG_INFO(
                "STEERING",
                "Zero position = " +
                String(steeringZero)
            );

            return;
        }
        // --------------------------------------------
        // CALIBRATION - SAVE
        // --------------------------------------------

        if(data == "SAVE")
        {
            steeringPreferences.begin(
                "steering",
                false
            );

            steeringPreferences.putInt(
                "zero",
                steeringZero
            );
            steeringPreferences.putInt(
                "angle",
                steeringAngle
            );

            steeringPreferences.end();

            LOG_INFO(
                "STEERING",
                "Calibration saved. Zero = " +
                String(steeringZero)
            );

            return;
        }

        // --------------------------------------------
        // INVALID CALIBRATION COMMAND
        // --------------------------------------------

        LOG_WARN(
            "STEERING",
            "Invalid calibration command: " + data
        );

        return;
    }
        
    /// ------------------------------------------------
    // NORMAL STEERING
    //
    // STEER:-100 ... +100
    // ------------------------------------------------

    if(command == "STEER")
    {
        int steeringValue = data.toInt();

        steeringValue = constrain(
            steeringValue,
            -100,
            100
        );

        int servoPosition;

        if(steeringValue < 0)
        {
            servoPosition = map(
                steeringValue,
                -100,
                0,
                steeringLeftLimit,
                steeringZero
            );
        }
        else
        {
            servoPosition = map(
                steeringValue,
                0,
                100,
                steeringZero,
                steeringRightLimit
            );
        }

        steeringServo.write(
            servoPosition
        );

        LOG_INFO(
            "STEERING",
            "Steering = " +
            String(steeringValue) +
            "  Servo = " +
            String(servoPosition)
        );

        return;
    }
    

    // ------------------------------------------------
    // Invalid command
    // ------------------------------------------------

    LOG_WARN(
        "STEERING",
        "Invalid steering command: " + data
    );
}