#ifndef _DATETIME_UTIL_H
#define _DATETIME_UTIL_H

#include <DS3231.h>
#include <Wire.h>
#include "EEPROMTool.h"
#include "GlobalConfigure.h"

class DateTimeUtil
{
private:
  bool h12Flag, pmFlag, century = false;
  DS3231 ds3231;

  DateTime datetime;

  /**
   * @brief 时间结构体
   *
   */
  Times times;

  /**
   * @brief 日期结构体
   *
   */
  Dates dates;

public:
  /**
   * 构造函数
   */
  DateTimeUtil();

  /**
   * 获取时间
   */
  Times getTimes();

  /**
   * 获取秒数
   */
  int getSecond();

  /**
   * 获取分钟
   */
  int getMinute();

  /**
   * 获取时刻
   */
  int getHour();

  /**
   * 获取日期
   */
  Dates getDates();

  /**
   * 获取温度信息
   */
  float getTemperature();

  /**
   * 根据datetime设置时间
   */
  void setDateTimes(long timestamp);

  /**
   * 获取时间戳信息
   */
  long getTimestamp();

  /**
   * 获取日期字符串
   */
  String getDateString();

  /**
   * 获取时间字符串
   */
  String getTimeString();

  /**
   * 输出日期时间
   */
  String getDateTimeString();

  /**
   * 获取倒计时的时间戳
   */
  long getCountdownTimestamp();

  /**
   * 保存倒计时的时间戳
   */
  void saveCountdownTimestamp(long timestamp);
};

#endif
