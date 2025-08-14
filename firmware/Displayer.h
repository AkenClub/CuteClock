#ifndef _DISPLAYER_H_
#define _DISPLAYER_H_

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#include "GlobalConfigure.h"

#define DATA_PIN D7
#define CLK_PIN D5
#define CS_PIN D6

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4

class Displayer
{
private:
    volatile int _hour_variable;
    volatile int _minute_variable;
    volatile int _second_variable;

    volatile int _day_variable = 0;
    volatile float _temperature_variable = 0.0f;
    String _temperature_door_variable = "";
    volatile int _normal_text_variable = 0;

    String _displayTmp;
    char *_displayDataTmp = new char[10];
    char *_displayDataTmpZone0 = new char[10];

public:
    MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);
    MD_MAX72XX mx = MD_MAX72XX(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

    void display_bitmap(int abscissa, int width, int bitmap_number);
    void display_bitmap(int abscissa, int width, uint8_t *bitmap);

    void init();
    void run();
    void clear();
    void showTime(Times times);
    void showDate(Dates dates);
    void showTemperature(float temperature);
    void showWifi();
    void showNormalText(int zone, String text);
    void showOutdoorWeather(WeatherData weatherData);
    void showContent(int zone, const char *content);

    void resetDateTemperature();

    void setClockBrightness(int value);
    void checkClockBrightnessByHour();
    void checkClockBrightnessByHour(int hour);
};

#endif
