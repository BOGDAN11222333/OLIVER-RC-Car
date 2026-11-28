
#include "CommandProcessor.h"

#include "BLE_Manager.h"
#include "WiFi_Manager.h"
#include "Logger.h"
#include "VehicleControl.h"


//************************************************************
// Process incoming command
//************************************************************

void CommandProcessor_process(String command)
{
    command.trim();

    LOG_INFO(
        "CMD",
        "Processing: " + command
    );


    //--------------------------------------------------------
    // TEST LONG MESSAGES RECEPTION
    //--------------------------------------------------------

    if (command.startsWith("TEST|"))
    {
        LOG_INFO("TEST", "Message received");

        LOG_INFO("TEST", command);

        LOG_INFO(
            "TEST",
            "Length = " + String(command.length())
        );

        Serial.print("HEX: ");

        for (int i = 0; i < command.length(); i++)
        {
            Serial.printf(
                "%02X ",
                (uint8_t)command[i]
            );
        }

        Serial.println();

        return;
    }


    //--------------------------------------------------------
    // PARSE COMMAND : DATA
    //--------------------------------------------------------

    String cmd  = command;
    String data = "";

    int separator = command.indexOf(':');

    if (separator >= 0)
    {
        cmd  = command.substring(0, separator);
        data = command.substring(separator + 1);

        cmd.trim();
        data.trim();
    }


    LOG_INFO(
        "CMD",
        "Command = " + cmd
    );

    LOG_INFO(
        "CMD",
        "Data = " + data
    );


    //--------------------------------------------------------
    // WIFI SCAN
    //--------------------------------------------------------

    if (cmd == "SCAN_WIFI")
    {
        WiFi_scanNetworks();

        return;
    }


    //--------------------------------------------------------
    // WIFI CONNECT
    //
    // Format:
    //
    // WIFI_CONNECT:SSID,PASSWORD
    //
    //--------------------------------------------------------

    if (cmd == "WIFI_CONNECT")
    {
        //----------------------------------------------------
        // Split DATA into SSID and password
        //----------------------------------------------------

        int comma = data.indexOf(',');

        if (comma < 0)
        {
            LOG_WARN(
                "CMD",
                "Invalid WIFI_CONNECT data"
            );

            BLE_send(
                "<WIFI_FAILED>"
            );

            return;
        }


        String ssid =
            data.substring(0, comma);

        String password =
            data.substring(comma + 1);


        ssid.trim();
        password.trim();


        //----------------------------------------------------
        // Log information
        //----------------------------------------------------

        LOG_INFO(
            "CMD",
            "SSID = " + ssid
        );

        LOG_INFO(
            "CMD",
            "Password received"
        );


        //----------------------------------------------------
        // Store WiFi credentials
        //----------------------------------------------------

        WiFi_setSSID(ssid);
        WiFi_setPassword(password);


        //----------------------------------------------------
        // Start connection
        //----------------------------------------------------

        LOG_INFO(
            "CMD",
            "Starting WiFi connection"
        );


        BLE_send(
            "<WIFI_CONNECTING>"
        );


        WiFi_connect();

        return;
    }
	 
    if (cmd == "LIGHTS")
    {
        VehicleControl_execute(
            cmd,
            data
        );

        return;
}
	//--------------------------------------------------------
    // DRIVE
    //
    // Format:
    //
    // DRIVE:100
    // DRIVE:50
    // DRIVE:0
    // DRIVE:-50
    // DRIVE:-100
    //--------------------------------------------------------

    if (cmd == "DRIVE")
    {
        VehicleControl_execute(
            cmd,
            data
        );

        return;
    }
    //--------------------------------------------------------
    // STEERING
    //
    // Format:
    //
    // STEER:LEFT
    // STEER:CENTER
    // STEER:RIGHT
    //--------------------------------------------------------

    if (cmd == "STEER")
    {
        VehicleControl_execute(
            cmd,
            data
        );

        return;
    }
    //--------------------------------------------------------
    // STEERING CALIBRATION
    //
    // Format:
    //
    // STEER_CAL:LEFT
    // STEER_CAL:RIGHT
    // STEER_CAL:ZERO
    // STEER_CAL:SAVE
    //--------------------------------------------------------

    if (cmd == "STEER_CAL")
    {
        VehicleControl_execute(
            cmd,
            data
        );

        return;
    }
    //--------------------------------------------------------
    // UNKNOWN COMMAND
    //--------------------------------------------------------

    LOG_WARN(
        "CMD",
        "Unknown command: " + command
    );
}


//************************************************************
// Command Processor initialization
//************************************************************

void CommandProcessor_begin()
{
    LOG_INFO(
        "CMD",
        "Command Processor Ready"
    );
}


//************************************************************
// Command Processor update
//************************************************************

//************************************************************
// Command Processor update
//************************************************************

void CommandProcessor_update()
{
    if (Serial.available() > 0)
    {
        String command = Serial.readStringUntil('\n');

        command.trim();

        if (command.length() == 0)
        {
            return;
        }

        // ----------------------------------------------------
        // Accept both:
        //
        // DRIVE:50
        //
        // and:
        //
        // <DRIVE:50>
        // ----------------------------------------------------

        if (command.startsWith("<") &&
            command.endsWith(">"))
        {
            command = command.substring(
                1,
                command.length() - 1
            );

            command.trim();
        }

        Serial.print("[Serial] Command: ");
        Serial.println(command);

        CommandProcessor_process(command);
    }
}
