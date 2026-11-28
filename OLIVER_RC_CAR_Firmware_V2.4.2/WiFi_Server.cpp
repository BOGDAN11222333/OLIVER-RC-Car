#include "WiFi_Server.h"

#include <WiFi.h>
#include <WebServer.h>

#include "Protocol.h"
#include "Logger.h"


//--------------------------------------------------
// HTTP Server
//--------------------------------------------------

static WebServer server(80);


//--------------------------------------------------
// Command Handler
//--------------------------------------------------
static void handleCommand()
{
    //------------------------------------------------
    // Check for plain text body
    //------------------------------------------------

    if (!server.hasArg("plain"))
    {
        LOG_WARN(
            "HTTP",
            "No command data received"
        );

        server.send(
            400,
            "text/plain",
            "NO_DATA"
        );

        return;
    }


    //------------------------------------------------
    // Get raw command
    //------------------------------------------------

    String command =
        server.arg("plain");

    command.trim();


    //------------------------------------------------
    // Log command
    //------------------------------------------------

    LOG_INFO(
        "HTTP",
        "RX: " + command
    );


    //------------------------------------------------
    // Pass directly to Protocol
    //------------------------------------------------

    Protocol::processIncoming(
        command
    );


    //------------------------------------------------
    // HTTP response
    //------------------------------------------------

    server.send(
        200,
        "text/plain",
        "OK"
    );
}


//--------------------------------------------------
// Server Initialization
//--------------------------------------------------

void WiFi_Server_begin()
{
    //------------------------------------------------
    // Register command endpoint
    //------------------------------------------------

    server.on(
        "/command",
        HTTP_POST,
        handleCommand
    );

    server.on(
        "/status",
        HTTP_GET,
        []()
        {
            LOG_INFO(
                "HTTP",
                "GET /status"
            );

        String response =
            "WIFI_OK:" +
            WiFi.localIP().toString();

        server.send(
            200,
            "text/plain",
            response
        );
    }
);

    //------------------------------------------------
    // Start HTTP server
    //------------------------------------------------

    server.begin();


    LOG_INFO(
        "HTTP",
        "HTTP Server Ready"
    );
}


//--------------------------------------------------
// Server Update
//--------------------------------------------------

void WiFi_Server_update()
{
    server.handleClient();
}