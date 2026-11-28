/********************************************************************
 * Project : OLIVER RC CAR
 * File    : BLE_Manager.cpp
 * Version : 2.0.0
 * Sprint  : 2.3 - Step 2
 * Patch   : BLE Protocol Compatibility
 * Purpose : BLE communication implementation
 ********************************************************************/

#include "BLE_Manager.h"
#include "WiFi_Manager.h"
#include "Logger.h"
#include "CommandProcessor.h"
#include "Protocol.h"

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


//--------------------------------------------------
// BLE UUID Definitions
//--------------------------------------------------

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"

#define RX_CHARACTERISTIC   "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define TX_CHARACTERISTIC   "6e400003-b5a3-f393-e0a9-e50e24dcca9e"

//--------------------------------------------------
// BLE Packet Configuration
//--------------------------------------------------

#define BLE_PACKET_SIZE 20

const String BLE_MSG_BEGIN = "@B";
const String BLE_MSG_END   = "@E";

//--------------------------------------------------
// BLE Objects
//--------------------------------------------------

static BLEServer *bleServer = nullptr;

static BLECharacteristic *rxCharacteristic = nullptr;

static BLECharacteristic *txCharacteristic = nullptr;

static unsigned long bleTestLastTime = 0; 
//--------------------------------------------------
// BLE TX Queue
//--------------------------------------------------

const int BLE_TX_QUEUE_SIZE = 60; // need improvement remaining from 2.4

String bleTxQueue[BLE_TX_QUEUE_SIZE];

int bleTxQueueHead = 0;
int bleTxQueueTail = 0;
int bleTxQueueCount = 0;

//--------------------------------------------------
// Current BLE transmission
//--------------------------------------------------

String bleTxMessage = "";
int bleTxIndex = 0;

bool bleTxActive = false;
bool bleTxBeginSent = false;

unsigned long bleTxLastSend = 0;

const unsigned long BLE_TX_INTERVAL = 100; // need improvement remaining from 2.4

//--------------------------------------------------
// Status
//--------------------------------------------------

static bool bleConnected = false;

static String receivedCommand = "";


//--------------------------------------------------
// Server Callbacks
//--------------------------------------------------

class BLE_ServerCallbacks :
    public BLEServerCallbacks
{

    void onConnect(
        BLEServer *server
    )
    {

        bleConnected = true;


        LOG_INFO(
            "BLE",
            "Client connected"
        );

    }


    void onDisconnect(
        BLEServer *server
    )
    {

        bleConnected = false;


        LOG_WARN(
            "BLE",
            "Client disconnected"
        );


        BLEDevice::startAdvertising();

    }

};


//--------------------------------------------------
// RX Callback
//--------------------------------------------------

class BLE_RX_Callbacks :
    public BLECharacteristicCallbacks
{

    void onWrite(BLECharacteristic *characteristic)
    {
    String value = characteristic->getValue();

        if(value.length() > 0)
        {
            value.trim();

            LOG_INFO("BLE", "RX: [" + value + "]");
            LOG_INFO("BLE", "Length = " + String(value.length()));

            for(int i = 0; i < value.length(); i++)
            {
                Serial.printf("%02X ", (uint8_t)value[i]);
            }
            Serial.println();

            Protocol::processIncoming(value);

            LOG_INFO(
                "BLE",
                "Negotiated MTU = " + String(BLEDevice::getMTU())
            );
        }
    }


};


//--------------------------------------------------
// BLE Initialization
//--------------------------------------------------

void BLE_begin()
{

    BLEDevice::init(
        BLE_DEVICE_NAME
    );


    bleServer =
        BLEDevice::createServer();

        BLEDevice::setMTU(100);


    bleServer->setCallbacks(
        new BLE_ServerCallbacks()
    );


    BLEService *service =
        bleServer->createService(
            SERVICE_UUID
        );


    //--------------------------------------------------
    // RX Characteristic
    //--------------------------------------------------

    rxCharacteristic =
        service->createCharacteristic(
            RX_CHARACTERISTIC,
            BLECharacteristic::PROPERTY_WRITE
        );


    rxCharacteristic->setCallbacks(
        new BLE_RX_Callbacks()
    );


    //--------------------------------------------------
    // TX Characteristic
    //--------------------------------------------------

    txCharacteristic =
        service->createCharacteristic(
            TX_CHARACTERISTIC,
            BLECharacteristic::PROPERTY_NOTIFY
        );


    txCharacteristic->addDescriptor(
        new BLE2902()
    );


    //--------------------------------------------------

    service->start();


    BLEAdvertising *advertising =
        BLEDevice::getAdvertising();


    advertising->addServiceUUID(
        SERVICE_UUID
    );


    advertising->setScanResponse(true);


    BLEDevice::startAdvertising();


    LOG_INFO(
        "BLE",
        "Advertising started"
    );

}


//--------------------------------------------------
// BLE Update
//--------------------------------------------------
void BLE_update()
{
    if (!bleConnected)
    {
        bleTxActive = false;

        bleTxMessage = "";
        bleTxIndex = 0;

        return;
    }

    //--------------------------------------------------
    // Start next queued message
    //--------------------------------------------------

    if (!bleTxActive && bleTxQueueCount > 0)
    {
        bleTxMessage = bleTxQueue[bleTxQueueHead];

        bleTxQueueHead++;

        if (bleTxQueueHead >= BLE_TX_QUEUE_SIZE)
            bleTxQueueHead = 0;

        bleTxQueueCount--;

        bleTxIndex = 0;

        bleTxActive = true;
        bleTxBeginSent = false;

        bleTxLastSend = 0;
    }

    //--------------------------------------------------
    // Nothing to transmit
    //--------------------------------------------------

    if (!bleTxActive)
        return;

    //--------------------------------------------------
    // Respect TX interval
    //--------------------------------------------------

    if (millis() - bleTxLastSend < BLE_TX_INTERVAL)
        return;

    //--------------------------------------------------
    // Begin message
    //--------------------------------------------------

    if (!bleTxBeginSent)
    {
        txCharacteristic->setValue(
            BLE_MSG_BEGIN.c_str()
        );

        txCharacteristic->notify();

        LOG_INFO("BLE", "TX Begin");

        bleTxBeginSent = true;

        bleTxLastSend = millis();

        return;
    }

    //--------------------------------------------------
    // Send payload chunk
    //--------------------------------------------------

    if (bleTxIndex < bleTxMessage.length())
    {
        String chunk =
            bleTxMessage.substring(
                bleTxIndex,
                bleTxIndex + BLE_PACKET_SIZE
            );

        txCharacteristic->setValue(
            chunk.c_str()
        );

        txCharacteristic->notify();

        LOG_INFO(
            "BLE",
            "TX Chunk: " + chunk
        );

        bleTxIndex += BLE_PACKET_SIZE;

        bleTxLastSend = millis();

        return;
    }

    //--------------------------------------------------
    // End message
    //--------------------------------------------------

    txCharacteristic->setValue(
        BLE_MSG_END.c_str()
    );

    txCharacteristic->notify();

    LOG_INFO(
        "BLE",
        "TX Complete: " + bleTxMessage
    );

    //--------------------------------------------------
    // Reset current transmission
    //--------------------------------------------------

    bleTxMessage = "";
    bleTxIndex = 0;

    bleTxActive = false;
    bleTxBeginSent = false;

    bleTxLastSend = millis();
}



//--------------------------------------------------
// Send Notification
//--------------------------------------------------

void BLE_send(String message)
{
    if (!bleConnected)
        return;

    //--------------------------------------------------
    // Check queue
    //--------------------------------------------------

    if (bleTxQueueCount >= BLE_TX_QUEUE_SIZE)
    {
        LOG_WARN("BLE", "TX queue full - message ignored");
        return;
    }

    //--------------------------------------------------
    // Add message to queue
    //--------------------------------------------------

    bleTxQueue[bleTxQueueTail] = message;

    bleTxQueueTail++;

    if (bleTxQueueTail >= BLE_TX_QUEUE_SIZE)
        bleTxQueueTail = 0;

    bleTxQueueCount++;

    LOG_INFO("BLE", "TX queued: " + message);
}



//--------------------------------------------------
// Connection Status
//--------------------------------------------------

bool BLE_isConnected()
{
    return bleConnected;
}