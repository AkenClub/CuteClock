#ifndef _LIGHT_UTIL_H_
#define _LIGHT_UTIL_H_

#include <SPI.h>
#include "GlobalConfigure.h"

// 需要引入 SPI 头文件，不然编译报错
#define ROOM_LIGHT D0

#define ROOM_LIGHT_ON "on"
#define ROOM_LIGHT_OFF "off"

class LightUtil
{
public:
    static void initLightState();
    static void setLightState(const String &state);
    static String getLightState();
};

#endif
