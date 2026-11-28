/********************************************************************
 * Project : OLIVER RC CAR
 * File    : Oliver_RC_Car.ino
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : Main firmware entry point
 ********************************************************************/

#include "config.h"
#include "Logger.h"
#include "BLE_Manager.h"
#include "WiFi_Manager.h"
#include "CommandProcessor.h"
#include "Protocol.h"
#include "VehicleControl.h"
#include "WiFi_Server.h"
#include "Lights.h"


//--------------------------------------------------
// Setup
//-------------------------------------------------

void setup()
{
    
    Logger_begin();

    Protocol::begin();
    
    LOG_INFO("SYSTEM", "Boot started");

    WiFi_begin();
    WiFi_Server_begin();

    BLE_begin();

    CommandProcessor_begin();
	VehicleControl_begin();
    Lights_begin();

    LOG_INFO("SYSTEM", "Initialization complete");
}


//--------------------------------------------------
// Main Loop
//--------------------------------------------------

void loop()
{
    BLE_update();

    WiFi_update();
    WiFi_Server_update();

    CommandProcessor_update(); 
    
}