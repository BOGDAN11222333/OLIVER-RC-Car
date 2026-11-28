#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <Arduino.h>

class Protocol
{

public:

    static void begin();

    static void processIncoming(const String &chunk);
	
	static void Protocol_send(const String &message);


private:

    static String rxBuffer;

    static void parseFrames();

};


#endif