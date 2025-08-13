#include "JsonUtilWeatherApi.h"

WeatherData JsonUtilWeatherApi::_weatherData = {"未知", "99"};

WeatherData JsonUtilWeatherApi::resolveWeather(String responseString)
{

    StaticJsonDocument<256> doc;

    DeserializationError error = deserializeJson(doc, responseString);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        _weatherData.temperature = "99";
        _weatherData.text = "未知";
        return _weatherData;
    }
    _weatherData.temperature = String((const char *)doc["temp"]);
    _weatherData.text = String((const char *)doc["weather"]);

    return _weatherData;
}
