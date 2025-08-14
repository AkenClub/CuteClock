#ifndef _JSON_UTIL_TCP_H_
#define _JSON_UTIL_TCP_H_

#include <ArduinoJson.h>

class JsonWifiServer
{
public:
    static String getAllStatusJson(boolean enable, int bright, float temperature);
    static String resolveSetRoomLightOder(String input);
    static int resolveSetClockBrightnessOder(String input);

    static String getSuccessResponse();
    static String getFailResponse(String errMsg);
};

#endif
