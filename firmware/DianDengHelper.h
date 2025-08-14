#ifndef _DIAN_DENG_HELPER_H_
#define _DIAN_DENG_HELPER_H_

#define BLINKER_PRINT Serial
#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
//启用热点 WIFI 一键配置功能，在 APP 配网完成后下次就会使用之前连接的 WIFI 信息
// #define BLINKER_APCONFIG // 暂时不走此方式，使用自己实现的

#include "GlobalConfigure.h"
#include "LightUtil.h"
#include "PCController.h"
#include "JsonUtilWeatherApi.h"

// 使用配置文件中的定义
inline char auth[] = UserBlinkerAuthKey;
// TODO:这里应该从 EEPROM 中读取
inline char ssid[] = UserWifiSSID;
inline char pswd[] = UserWifiPassword;

class DianDengHelper
{
public:
    Times times = {99, 99, 99};
    Dates dates = {2000, 1, 1};

    void init();
    void run();

    Times getTime();
    Dates getDates();

    boolean isNtpCorrectly();

    int getSecond();
    int getHour();
    int getMinute();

    uint32 getTimestamp();

    void getWeather();
};

#endif
