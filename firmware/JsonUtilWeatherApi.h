#ifndef _JSON_UTIL_WEATHER_API_H_
#define _JSON_UTIL_WEATHER_API_H_

#include <ArduinoJson.h>
#include "GlobalConfigure.h"

class JsonUtilWeatherApi
{
private:
    static WeatherData _weatherData;

public:
    static WeatherData resolveWeather(String responseString);
};

#endif
