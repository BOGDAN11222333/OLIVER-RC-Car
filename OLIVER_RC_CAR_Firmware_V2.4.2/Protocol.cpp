#include "Protocol.h"
#include "Logger.h"
#include "WiFi_Manager.h"
#include "CommandProcessor.h"
#include "BLE_Manager.h"

String Protocol::rxBuffer = "";


void Protocol::begin()
{
    LOG_INFO("Protocol", "Protocol initialized");
}



void Protocol::processIncoming(const String &chunk)
{
    LOG_INFO(
        "Protocol",
        "RX chunk: [" + chunk + "]"
    );

    rxBuffer += chunk;

    if (rxBuffer.length() > 512)
    {
        LOG_WARN(
            "Protocol",
            "RX buffer overflow"
        );

        rxBuffer = "";
        return;
    }

    parseFrames();
}



void Protocol::parseFrames()
{
    while (true)
    {
        int start = rxBuffer.indexOf('<');

        if (start < 0)
        {
            rxBuffer = "";
            return;
        }

        int end = rxBuffer.indexOf('>', start);

        if (end < 0)
        {
            if (start > 0)
            {
                rxBuffer.remove(0, start);
            }
            return;
        }

        String frame =
            rxBuffer.substring(
                start + 1,
                end
            );

        rxBuffer.remove(
            0,
            end + 1
        );

        LOG_INFO(
            "Protocol",
            "Frame: [" + frame + "]"
        );

        if(frame.length() > 0)
        {
            CommandProcessor_process(frame);
        }
    }
}





void Protocol::Protocol_send(const String &message)
{
    String frame = "<";
    frame += message;
    frame += ">";

    BLE_send(frame);
}