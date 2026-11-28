/********************************************************************
 * Project : OLIVER RC CAR
 * File    : WiFi_Manager.cpp
 * Version : 1.1
 * Firmware: 2.0.0
 * Sprint  : 2.3 Step 2
 * Purpose : Reliable WiFi State Machine
 ********************************************************************/

#include "WiFi_Manager.h"
#include "Logger.h"
#include "Protocol.h"
#include "BLE_Manager.h"

#include <WiFi.h>


//************************************************************
// Internal Variables
//************************************************************

static String wifiSSID = "";

static String wifiPassword = "";

static WiFiState wifiState = WIFI_IDLE;

static unsigned long stateTimer = 0;

static uint8_t reconnectCounter = 0;


//************************************************************
// Internal Function Prototypes
//************************************************************

static void setState(WiFiState newState);

static void startConnection();

static void connectionSucceeded();

static void connectionFailed();

static void reconnect();


//************************************************************
// State Change Helper
//************************************************************

static void setState(WiFiState newState)
{

    if (wifiState == newState)
        return;

    String oldName;
    String newName;

    switch (wifiState)
    {
        case WIFI_IDLE:
            oldName = "IDLE";
            break;

        case WIFI_CONNECTING:
            oldName = "CONNECTING";
            break;

        case WIFI_CONNECTED:
            oldName = "CONNECTED";
            break;

        case WIFI_RECONNECTING:
            oldName = "RECONNECTING";
            break;

        case WIFI_FAILED:
            oldName = "FAILED";
            break;
    }

    switch (newState)
    {
        case WIFI_IDLE:
            newName = "IDLE";
            break;

        case WIFI_CONNECTING:
            newName = "CONNECTING";
            break;

        case WIFI_CONNECTED:
            newName = "CONNECTED";
            break;

        case WIFI_RECONNECTING:
            newName = "RECONNECTING";
            break;

        case WIFI_FAILED:
            newName = "FAILED";
            break;
    }

    LOG_INFO(
        "WIFI",
        "State: " + oldName + " -> " + newName
    );

    wifiState = newState;

    stateTimer = millis();

}


//************************************************************
// WiFi Initialization
//************************************************************

void WiFi_begin()
{

    WiFi.mode(WIFI_STA);

    WiFi.disconnect(true);

    delay(100);

    setState(WIFI_IDLE);

    LOG_INFO(
        "WIFI",
        "WiFi Manager Ready"
    );

}
//************************************************************
// Start New WiFi Connection
//************************************************************

void WiFi_beginConnection(
    String ssid,
    String password
)
{

    //--------------------------------------------------------
    // Ignore empty SSID
    //--------------------------------------------------------

    if (ssid.length() == 0)
    {
        LOG_ERROR(
            "WIFI",
            "SSID is empty"
        );

        Protocol::Protocol_send("<WIFI_FAILED>");

        return;
    }


    //--------------------------------------------------------
    // Already connected to this network?
    //--------------------------------------------------------

    if (wifiState == WIFI_CONNECTED &&
        WiFi.SSID() == ssid)
    {

        LOG_INFO(
            "WIFI",
            "Already connected to " + ssid
        );

        Protocol::Protocol_send(
            "WIFI_CONNECTED:" +
            WiFi.localIP().toString()
        );

        return;
    }


    //--------------------------------------------------------
    // Store new credentials
    //--------------------------------------------------------

    wifiSSID = ssid;

    wifiPassword = password;

    reconnectCounter = 0;


    LOG_INFO(
        "WIFI",
        "Connection request received"
    );

    LOG_INFO(
        "WIFI",
        "SSID : " + wifiSSID
    );
    Serial.print("SSID HEX: ");

    for (int i = 0; i < wifiSSID.length(); i++)
    {
        Serial.printf("%02X ", (uint8_t)wifiSSID[i]);
    }

    Serial.println();

    //--------------------------------------------------------
    // Disconnect previous connection
    //--------------------------------------------------------

    WiFi.disconnect(true);

    delay(100);


    //--------------------------------------------------------
    // Start connection
    //--------------------------------------------------------

    startConnection();

}



//************************************************************
// Internal Connection Function
//************************************************************

static void startConnection()
{

    LOG_INFO(
        "WIFI",
        "Starting WiFi..."
    );


    WiFi.begin(
        wifiSSID.c_str(),
        wifiPassword.c_str()
    );


    reconnectCounter = 0;

    setState(
        WIFI_CONNECTING
    );

}



//************************************************************
// Successful Connection
//************************************************************

static void connectionSucceeded()
{

    reconnectCounter = 0;

    setState(
        WIFI_CONNECTED
    );


    LOG_INFO(
        "WIFI",
        "Connected"
    );


    LOG_INFO(
        "WIFI",
        "SSID : " +
        WiFi.SSID()
    );


    LOG_INFO(
        "WIFI",
        "IP   : " +
        WiFi.localIP().toString()
    );


    LOG_INFO(
        "WIFI",
        "RSSI : " +
        String(WiFi.RSSI()) +
        " dBm"
    );


    Protocol::Protocol_send(
        "WIFI_CONNECTED:" +
        WiFi.localIP().toString()
    );

}



//************************************************************
// Connection Failed
//************************************************************

static void connectionFailed()
{

    LOG_ERROR(
        "WIFI",
        "Connection timeout"
    );


    WiFi.disconnect(true);

    Protocol::Protocol_send(
        "<WIFI_FAILED>"
    );


    setState(
        WIFI_IDLE
    );

}
//************************************************************
// WiFi State Machine
//************************************************************

void WiFi_update()
{

    switch (wifiState)
    {

        //----------------------------------------------------
        // IDLE
        //----------------------------------------------------

        case WIFI_IDLE:
        {
            break;
        }


        //----------------------------------------------------
        // CONNECTING
        //----------------------------------------------------

        case WIFI_CONNECTING:
        {
            // temporarly add .........clear at one point...............
            //.................................
            static wl_status_t lastStatus = WL_IDLE_STATUS;

            wl_status_t status = WiFi.status();

            if (status != lastStatus)
            {
                LOG_INFO(
                    "WIFI",
                    "Status = " + String(status)
                );

                lastStatus = status;
            }
            //-----------------end of clearance-----------------
            //-------------------------------------
            if (WiFi.status() == WL_CONNECTED)
            {
                connectionSucceeded();
                break;
            }


            if (millis() - stateTimer >= WIFI_CONNECT_TIMEOUT)
            {
                connectionFailed();
                break;
            }

            break;
        }


        //----------------------------------------------------
        // CONNECTED
        //----------------------------------------------------

        case WIFI_CONNECTED:
        {

            if (WiFi.status() != WL_CONNECTED)
            {

                LOG_WARN(
                    "WIFI",
                    "WiFi connection lost"
                );

                reconnect();

            }

            break;
        }


        //----------------------------------------------------
        // RECONNECTING
        //----------------------------------------------------

        case WIFI_RECONNECTING:
        {

            if (WiFi.status() == WL_CONNECTED)
            {

                reconnectCounter = 0;

                LOG_INFO(
                    "WIFI",
                    "Reconnected"
                );

                Protocol::Protocol_send(
                    "WIFI_RECONNECTED:" +
                    WiFi.localIP().toString()
                );

                setState(
                    WIFI_CONNECTED
                );

                break;
            }


            if (millis() - stateTimer >= WIFI_RECONNECT_TIMEOUT)
            {

                reconnectCounter++;

                LOG_WARN(
                    "WIFI",
                    "Reconnect attempt " +
                    String(reconnectCounter)
                );


                if (reconnectCounter >= WIFI_MAX_RETRIES)
                {

                    LOG_ERROR(
                        "WIFI",
                        "Reconnect failed"
                    );

                    Protocol::Protocol_send(
                        "WIFI_LOST"
                    );

                    WiFi.disconnect(true);

                    setState(
                        WIFI_IDLE
                    );

                    break;

                }


                WiFi.disconnect(true);

                delay(100);

                WiFi.begin(
                    wifiSSID.c_str(),
                    wifiPassword.c_str()
                );

                stateTimer = millis();

            }

            break;
        }


        //----------------------------------------------------
        // FAILED
        //----------------------------------------------------

        case WIFI_FAILED:
        {

            setState(
                WIFI_IDLE
            );

            break;
        }

    }

}
//************************************************************
// Start Reconnection
//************************************************************

static void reconnect()
{

    LOG_INFO(
        "WIFI",
        "Starting reconnect..."
    );

    WiFi.disconnect(true);

    delay(100);

    WiFi.begin(
        wifiSSID.c_str(),
        wifiPassword.c_str()
    );

    reconnectCounter = 0;

    setState(
        WIFI_RECONNECTING
    );

}


//************************************************************
// Disconnect WiFi
//************************************************************

void WiFi_disconnect()
{

    LOG_INFO(
        "WIFI",
        "Disconnect requested"
    );

    WiFi.disconnect(true);

    wifiSSID = "";

    wifiPassword = "";

    reconnectCounter = 0;

    setState(
        WIFI_IDLE
    );

}


//************************************************************
// Connection Status
//************************************************************

bool WiFi_isConnected()
{

    return (
        wifiState == WIFI_CONNECTED
    );

}


//************************************************************
// Current State
//************************************************************

WiFiState WiFi_getState()
{

    return wifiState;

}
//************************************************************
// Store WiFi SSID
//************************************************************

void WiFi_setSSID(String ssid)
{
    wifiSSID = ssid;

    LOG_INFO(
        "WIFI",
        "SSID stored: " + wifiSSID
    );
}


//************************************************************
// Store WiFi Password
//************************************************************

void WiFi_setPassword(String password)
{
    wifiPassword = password;

    LOG_INFO(
        "WIFI",
        "Password stored"
    );
}


//************************************************************
// Connect using stored credentials
//************************************************************

void WiFi_connect()
{
    WiFi_beginConnection(
        wifiSSID,
        wifiPassword
    );
}

//************************************************************
// IP Address
//************************************************************

String WiFi_getIP()
{

    if (WiFi.status() != WL_CONNECTED)
    {
        return "";
    }

    return WiFi.localIP().toString();

}
//************************************************************
// WiFi Network Scan
//************************************************************

void WiFi_scanNetworks()
{

    LOG_INFO(
        "WIFI",
        "Starting WiFi scan"
    );


    Protocol::Protocol_send(
        "WIFI_LIST_START"
    );


    int networks =
        WiFi.scanNetworks();


    if(networks <= 0)
    {

        LOG_WARN(
            "WIFI",
            "No networks found"
        );


        Protocol::Protocol_send(
            "WIFI_LIST_EMPTY"
        );

    }
    else
    {

        LOG_INFO(
            "WIFI",
            "Networks found: " +
            String(networks)
        );


        for(int i = 0; i < networks; i++)
        {

            String message =
                "WIFI:" +
                WiFi.SSID(i) +
                "," +
                String(WiFi.RSSI(i));


            LOG_INFO(
                "WIFI",
                message
            );


            Protocol::Protocol_send(
                message
            );


            delay(20);

        }

    }


    Protocol::Protocol_send(
        "WIFI_LIST_END"
    );


WiFi.scanDelete();


    LOG_INFO(
        "WIFI",
        "Scan completed"
    );

}