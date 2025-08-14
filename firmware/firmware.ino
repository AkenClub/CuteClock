#include <ESP8266WiFi.h>
#include <TickTwo.h>
#include "DateTimeUtil.h"
#include "Displayer.h"
#include "DianDengHelper.h"
#include "GlobalConfigure.h"
#include "LightUtil.h"
#include "UdpUtil.h"
#include "WifiServer.h"

// 引入配置文件
#ifndef _USER_CONFIG_H_
#include "UserConfig.h"  // 用户需要根据 UserConfig.example.h 创建此文件
#endif

enum DisplayMode
{
  MODE_TIME,
  MODE_DATE,
  // 室内温度
  MODE_TEMPERATURE,
  // 室外温度
  MODE_OUTDOOR_FEELING_TEMPERATURE,
  MODE_LOVE
};

// 尝试连接 WIFI 的最大次数
int TRY_CONNECT_TIME_MAX = UserWifiConnectMaxRetry;

// 是否为离线模式
boolean IS_OFFLINE_MODE = false;

void checkNtpCorrect();
TickTwo mNtpTicker(checkNtpCorrect, UserNtpCheckInterval);

void updateWeather();
TickTwo mWeatherTicker(updateWeather, UserWeatherUpdateInterval);

void broadcastUdp();
TickTwo mUdpTicker(broadcastUdp, UserUdpBroadcastInterval);

Displayer mDisplayer;
DianDengHelper mDianDengHelper;
DateTimeUtil mDateTimeUtil;
UdpUtil mUdpUtil;

int _second;
int _hour;
int _minute;

String _loveStr = String(UserLoveDisplayText);

// 初始化时钟显示模式
DisplayMode mDisplayModeNow = MODE_TIME;

// 每秒钟定时任务回调 - 设置时钟
void checkDisplayMode()
{
  // 根据秒数来设置显示模式

  if (IS_OFFLINE_MODE)
  {
    _second = mDateTimeUtil.getSecond();
    _minute = mDateTimeUtil.getMinute();
  }
  else
  {
    _second = mDianDengHelper.getSecond();
    _minute = mDianDengHelper.getMinute();
  }
  if (0 != _minute % 2)
  {
    switch (_second)
    {
    case 20:

      if ((0 != _minute % 2) && mDisplayModeNow != MODE_LOVE)
      {
        mDisplayer.resetDateTemperature();
        mDisplayModeNow = MODE_LOVE;
        mDisplayer.clear();
        Serial.println("显示 LOVE");
      }
      break;
    case 25:
      if (mDisplayModeNow != MODE_DATE)
      {
        mDisplayModeNow = MODE_DATE;
        // 重置，不然第二次显示日期的时候就不正常
        mDisplayer.resetDateTemperature();
        mDisplayer.clear();
        Serial.println("显示日期");
      }
      break;
    case 30:
      if (mDisplayModeNow != MODE_TIME)
      {
        mDisplayModeNow = MODE_TIME;
        mDisplayer.clear();
        Serial.println("显示时钟");
      }
      break;
    default:
      // 其余时间不设置，便于远程设置显示模式之后不被覆盖
      break;
    }
  }
  else
  {
    switch (_second)
    {
    case 30:
      if (mDisplayModeNow != MODE_TEMPERATURE)
      {
        mDisplayModeNow = MODE_TEMPERATURE;
        // 同上原理
        mDisplayer.resetDateTemperature();
        mDisplayer.clear();
        Serial.println("显示室温");
      }
      break;
    case 35:
      if (mDisplayModeNow != MODE_OUTDOOR_FEELING_TEMPERATURE)
      {
        mDisplayModeNow = MODE_OUTDOOR_FEELING_TEMPERATURE;
        mDisplayer.resetDateTemperature();
        mDisplayer.clear();
        Serial.println("显示室外温度");
      }
      break;
    case 40:
      if (mDisplayModeNow != MODE_TIME)
      {
        mDisplayModeNow = MODE_TIME;
        mDisplayer.clear();
        Serial.println("显示时钟");
      }
      break;
    default:
      // 其余时间不设置，便于远程设置显示模式之后不被覆盖
      break;
    }
  }

  handleDisplayMode();
}

// 处理显示模式
void handleDisplayMode()
{
  switch (mDisplayModeNow)
  {
  case MODE_TIME:
    showTime();
    break;
  case MODE_DATE:
    showDate();
    break;
  case MODE_TEMPERATURE:
    showTemperature();
    break;
  case MODE_OUTDOOR_FEELING_TEMPERATURE:
    mDisplayer.showOutdoorWeather(GLOBAL_OUTDOOR_WEATHER);
    break;
  case MODE_LOVE:
    mDisplayer.showNormalText(0, _loveStr);
    break;
  default:
    break;
  }
}

void showTime()
{
  Times _times;
  if (IS_OFFLINE_MODE)
  {
    // 离线模式更新时间，则使用 DS3231 模块保存的时间
    _times = mDateTimeUtil.getTimes();
  }
  else
  {
    _times = mDianDengHelper.getTime();
  }

  mDisplayer.showTime(_times);
}

void showDate()
{
  Dates _dates;
  if (IS_OFFLINE_MODE)
  {
    _dates = mDateTimeUtil.getDates();
  }
  else
  {
    _dates = mDianDengHelper.getDates();
  }
  mDisplayer.showDate(_dates);
}

void showTemperature()
{
  mDisplayer.showTemperature(mDateTimeUtil.getTemperature());
}

// 检查时钟亮度是否被远程修改
void checkClockBrightnessChange()
{
  // 检测全局设置的时钟亮度（可能被远程修改）和当前设置的时钟亮度是否一致，否则设置亮度
  if (GLOBAL_CLOCK_BRIGHTNESS != GLOBAL_CLOCK_BRIGHTNESS_NOW)
  {
    mDisplayer.setClockBrightness(GLOBAL_CLOCK_BRIGHTNESS);
  }
}

// 检测 NTP 正确性，准确后才设置亮度
void checkNtpCorrect()
{
  int _hour = mDianDengHelper.getHour();
  if (_hour != -1)
  {
    Serial.println("NTP 获取时钟准确，设置亮度");
    mDisplayer.checkClockBrightnessByHour(_hour);
    mNtpTicker.stop();
    Serial.println("NTP 定时器解除");

    // 设置 日期和时间到 DS3231
    uint32_t timestamp = mDianDengHelper.getTimestamp();
    Serial.print("设置DS3231 timestamp：");
    Serial.println(timestamp);
    mDateTimeUtil.setDateTimes(timestamp);

    // 这时一般 Blinker 初始化完成，可以获取天气
    mDianDengHelper.getWeather();

    return;
  }
  Serial.println("NTP 等待准确...");
}

// 更新室外天气
void updateWeather()
{
  mDianDengHelper.getWeather();
}

// 发送 UDP 广播
void broadcastUdp()
{
  mUdpUtil.broadcast();
}

void setup()
{
  // 延时2s，让电源稍微稳定一下
  delay(2000);

  Serial.begin(115200);

  // 需要先初始化 LED 屏幕
  mDisplayer.init();
  mDisplayer.setClockBrightness(GLOBAL_CLOCK_BRIGHTNESS_NOW);

  LightUtil::initLightState();

  // TODO: 检测 EEPROM 里面有没有存到 WIFI 信息，有则联网，没有则开启配网模式
  WiFi.begin(UserWifiSSID, UserWifiPassword);

  // 尝试连接 WIFI 次数
  int tryConnectTimeNow = 0;

  while (WiFi.status() != WL_CONNECTED && tryConnectTimeNow < TRY_CONNECT_TIME_MAX)
  {
    delay(500);
    Serial.print(".");
    mDisplayer.showWifi();
    tryConnectTimeNow++;
  }
  mDisplayer.clear();

  if (tryConnectTimeNow >= TRY_CONNECT_TIME_MAX)
  {
    Serial.println("尝试连接 WIFI 次数已满，进入离线模式");

    IS_OFFLINE_MODE = true;

    mDisplayer.checkClockBrightnessByHour(mDateTimeUtil.getTimes().h);
    return;
  }

  // 连接 WIFI 成功，打印信息，开始连接点灯科技相关
  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());

  mDianDengHelper.init();
  mNtpTicker.start();
  mWeatherTicker.start();

  if (mUdpUtil.init())
  {
    mUdpTicker.start();
  }

  wifiServerInit();
}

void loop()
{
  if (!IS_OFFLINE_MODE)
  {
    // 非离线模式
    mDianDengHelper.run();
    checkClockBrightnessChange();
    mNtpTicker.update();
    mWeatherTicker.update();
    mUdpTicker.update();
    wifiServerRun();
  }
  checkDisplayMode();

  mDisplayer.run();
}
