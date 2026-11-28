/********************************************************************
 * Project : OLIVER RC CAR
 * File    : Logger.cpp
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : Logging implementation
 ********************************************************************/

#include "Logger.h"


//--------------------------------------------------
// Internal Logger Function
//--------------------------------------------------

static void Logger_print(
    const char *level,
    const String &module,
    const String &message
)
{
    if (!LOG_ENABLE)
        return;


    Serial.print("[");

    Serial.print(level);

    Serial.print("][");

    Serial.print(module);

    Serial.print("] ");

    Serial.println(message);
}


//--------------------------------------------------
// Logger Initialization
//--------------------------------------------------

void Logger_begin()
{
    Serial.begin(SERIAL_BAUDRATE);


    unsigned long startTime = millis();


    while (!Serial &&
           millis() - startTime < 2000)
    {
        // Wait for USB Serial
    }


    Serial.println();
    Serial.println("====================================");

    Serial.print(FW_NAME);

    Serial.print(" Firmware ");

    Serial.println("FW_BOARD");

    Serial.println("Logger started");

    Serial.println("====================================");
}


//--------------------------------------------------
// INFO
//--------------------------------------------------

void Logger_info(
    const String &module,
    const String &message
)
{
    if (LOG_LEVEL <= LOG_LEVEL_INFO)
    {
        Logger_print(
            "INFO",
            module,
            message
        );
    }
}



//--------------------------------------------------
// WARNING
//--------------------------------------------------

void Logger_warn(
    const String &module,
    const String &message
)
{
    if (LOG_LEVEL <= LOG_LEVEL_WARN)
    {
        Logger_print(
            "WARN",
            module,
            message
        );
    }
}


//--------------------------------------------------
// ERROR
//--------------------------------------------------

void Logger_error(
    const String &module,
    const String &message
)
{
    if (LOG_LEVEL <= LOG_LEVEL_ERROR)
    {
        Logger_print(
            "ERROR",
            module,
            message
        );
    }
}