/********************************************************************
 * Project : OLIVER RC CAR
 * File    : WiFi_Manager.h
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : WiFi communication interface
 ********************************************************************/

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H


#include <Arduino.h>
#include <WiFi.h>


//--------------------------------------------------
// WiFi States
//--------------------------------------------------

enum WiFiState
{
    WIFI_IDLE,

    WIFI_CONNECTING,

    WIFI_CONNECTED,

    WIFI_RECONNECTING,

    WIFI_FAILED
};


//--------------------------------------------------
// WiFi Functions
//--------------------------------------------------

void WiFi_begin();

void WiFi_update();

bool WiFi_isConnected();

WiFiState WiFi_getState();


//--------------------------------------------------
// WiFi Scan
//--------------------------------------------------

void WiFi_scanNetworks();


//--------------------------------------------------
// WiFi Credentials
//--------------------------------------------------

void WiFi_setSSID(
    String ssid
);

void WiFi_setPassword(
    String password
);


//--------------------------------------------------
// WiFi Connection
//--------------------------------------------------

void WiFi_connect();

void WiFi_disconnect();


//--------------------------------------------------
// Status
//--------------------------------------------------

void WiFi_update();

String WiFi_getIP();


#endif