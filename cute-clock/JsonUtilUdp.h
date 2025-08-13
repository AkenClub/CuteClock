#ifndef _JSON_UTIL_UDP_H_
#define _JSON_UTIL_UDP_H_

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include "GlobalConfigure.h"

class JsonUtilUdp
{
public:
    static String getBroadcastJson();
};

#endif
