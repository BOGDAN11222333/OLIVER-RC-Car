/********************************************************************
 * Project : OLIVER RC CAR
 * File    : config.h
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Purpose : Global configuration
 ********************************************************************/

#ifndef CONFIG_H
#define CONFIG_H


//--------------------------------------------------
// Firmware Information
//--------------------------------------------------

#define FW_NAME            "OLIVER RC CAR"
#define FW_VERSION         "2.3.1"
#define FW_BOARD           "ESP32-S3"


//--------------------------------------------------
// Serial
//--------------------------------------------------

#define SERIAL_BAUDRATE 115200


//--------------------------------------------------
// Logger
//--------------------------------------------------

#define LOG_ENABLE true

enum LogLevel
{
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_WARN = 1,
    LOG_LEVEL_ERROR = 2
};

#define LOG_LEVEL LOG_LEVEL_INFO


//--------------------------------------------------
// WiFi Configuration
//--------------------------------------------------

#define WIFI_CONNECT_TIMEOUT       10000UL

#define WIFI_RECONNECT_TIMEOUT      5000UL

#define WIFI_MAX_RETRIES                 5


//--------------------------------------------------
// Communication
//--------------------------------------------------

#define HEARTBEAT_TIMEOUT            500UL


//------------------------------------------------------
// Protocol
//------------------------------------------------------

#define FRAME_START        '<'
#define FRAME_END          '>'

#define RX_BUFFER_MAX_SIZE 512


//--------------------------------------------------
// BLE
//--------------------------------------------------

#define BLE_DEVICE_NAME    "OLIVER RC CAR"

#define BLE_SERVICE_UUID   "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define BLE_RX_UUID        "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define BLE_TX_UUID        "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

#define BLE_MTU            100


#endif