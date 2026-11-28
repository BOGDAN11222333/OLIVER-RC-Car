/********************************************************************
 * Project : OLIVER RC CAR
 * File    : Logger.h
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : Logging interface
 ********************************************************************/

#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include "config.h"


//--------------------------------------------------
// Logger Functions
//--------------------------------------------------

void Logger_begin();

void Logger_info(
    const String &module,
    const String &message
);

void Logger_warn(
    const String &module,
    const String &message
);

void Logger_error(
    const String &module,
    const String &message
);


//--------------------------------------------------
// Simplified Macros
//--------------------------------------------------

#define LOG_INFO(module, message) \
        Logger_info(module, message)

#define LOG_WARN(module, message) \
        Logger_warn(module, message)

#define LOG_ERROR(module, message) \
        Logger_error(module, message)
        

#endif