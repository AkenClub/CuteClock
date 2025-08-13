#include "GlobalConfigure.h"

int CITY_CODE = UserWeatherCityCode;

int WIFI_SERVER_PORT = UserHttpServerPort;

// 时钟最低亮度
int GLOBAL_CLOCK_BRIGHTNESS_LOW = UserClockBrightnessMin;
// 时钟最高亮度
int GLOBAL_CLOCK_BRIGHTNESS_MAX = UserClockBrightnessMax;

// 全局的时钟亮度，提供给 点灯科技 / TCP小程序 等去设置。
int GLOBAL_CLOCK_BRIGHTNESS = UserClockBrightnessDefault;

// 当前时钟设置的亮度，初始值和全局的一致。
int GLOBAL_CLOCK_BRIGHTNESS_NOW = GLOBAL_CLOCK_BRIGHTNESS;

// 全局的灯状态，用于给远程获取 /
String GLOBAL_ROOM_LIGHT_STATUS = "";

// 全局的房间室内温度
float GLOBAL_ROOM_TEMPERATURE = 0.0f;

// 全局的室外温度
WeatherData GLOBAL_OUTDOOR_WEATHER = {"未知", "99"};

void SET_GLOBAL_CLOCK_BRIGHTNESS(int value)
{
    GLOBAL_CLOCK_BRIGHTNESS = value;
    GLOBAL_CLOCK_BRIGHTNESS_NOW = value;
}

void SET_GLOBAL_ROOM_TEMPERATURE(float value)
{
    GLOBAL_ROOM_TEMPERATURE = value;
}

void SET_GLOBAL_OUTDOOR_WEATHER(WeatherData weatherData)
{
    GLOBAL_OUTDOOR_WEATHER.temperature = weatherData.temperature + "*";
    GLOBAL_OUTDOOR_WEATHER.text = weatherData.text;
}
