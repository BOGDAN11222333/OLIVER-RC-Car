/********************************************************************
 * Project : OLIVER RC CAR
 * File    : BLE_Manager.h
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Patch   : BLE Protocol Compatibility
 * Purpose : BLE communication interface
 ********************************************************************/

#ifndef BLE_MANAGER_H
#define BLE_MANAGER_H


#include <Arduino.h>


//--------------------------------------------------
// BLE Functions
//--------------------------------------------------

void BLE_begin();

void BLE_update();

void BLE_send(String message);

bool BLE_isConnected();


#endif