/********************************************************************
 * Project : OLIVER RC CAR
 * File    : CommandProcessor.h
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : Command processing interface
 ********************************************************************/

#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H


#include <Arduino.h>


//--------------------------------------------------
// Command Processor Functions
//--------------------------------------------------

void CommandProcessor_begin();

void CommandProcessor_update();

void CommandProcessor_process(
    String command
);


#endif