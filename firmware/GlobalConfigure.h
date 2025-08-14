#ifndef _GLOBAL_CONFIGURE_H_
#define _GLOBAL_CONFIGURE_H_

#include "WString.h"
#include "UserConfig.h"

struct WeatherData
{
    String text;
    String temperature;
};

struct Times
{
    uint8_t h;
    uint8_t m;
    uint8_t s;
};

struct Dates
{
    uint16_t y;
    uint8_t m;
    uint8_t d;
};

// 获取天气用的城市代码
extern int CITY_CODE;
extern int WIFI_SERVER_PORT;

extern int GLOBAL_CLOCK_BRIGHTNESS;
extern int GLOBAL_CLOCK_BRIGHTNESS_LOW;
extern int GLOBAL_CLOCK_BRIGHTNESS_MAX;
extern int GLOBAL_CLOCK_BRIGHTNESS_NOW;

// 需要引入 WString 头文件，不然编译报错
// 房间灯的状态
extern String GLOBAL_ROOM_LIGHT_STATUS;

extern float GLOBAL_ROOM_TEMPERATURE;

extern WeatherData GLOBAL_OUTDOOR_WEATHER;

extern void SET_GLOBAL_CLOCK_BRIGHTNESS(int value);

extern void SET_GLOBAL_ROOM_TEMPERATURE(float value);

extern void SET_GLOBAL_OUTDOOR_WEATHER(WeatherData value);

#endif
