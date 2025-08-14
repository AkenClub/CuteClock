#include "DateTimeUtil.h"

DateTimeUtil::DateTimeUtil()
{
  Wire.begin();
}

int DateTimeUtil::getSecond()
{
  return ds3231.getSecond();
}

int DateTimeUtil::getHour()
{
  return ds3231.getHour(h12Flag, pmFlag);
}

int DateTimeUtil::getMinute()
{
  return ds3231.getMinute();
}

Times DateTimeUtil::getTimes()
{
  times.h = getHour();
  times.m = ds3231.getMinute();
  times.s = ds3231.getSecond();
  return times;
}

Dates DateTimeUtil::getDates()
{
  dates.y = 2000 + ds3231.getYear();
  dates.m = ds3231.getMonth(century);
  dates.d = ds3231.getDate();
  return dates;
}

float DateTimeUtil::getTemperature()
{
  // 返回如：2995 = 29.95
  return ds3231.getTemperature();
}

void DateTimeUtil::setDateTimes(long timestamp)
{
  datetime = DateTime(timestamp);
  ds3231.setYear(datetime.year() % 100);
  ds3231.setMonth(datetime.month());
  ds3231.setDate(datetime.day());
  // 添加一个时区，在 DateTime 转换之后比正确值少 8
  ds3231.setHour(datetime.hour() + 8);
  ds3231.setMinute(datetime.minute());
  ds3231.setSecond(datetime.second());
}

long DateTimeUtil::getTimestamp()
{
  return RTClib::now().unixtime();
}

String DateTimeUtil::getDateTimeString()
{
  String result;
  result += getDateString();
  result += "_";
  result += getTimeString();
  return result;
}

String DateTimeUtil::getTimeString()
{
  String result;
  result += ds3231.getHour(h12Flag, pmFlag);
  result += ":";
  result += ds3231.getMinute();
  result += ":";
  result += ds3231.getSecond();
  return result;
}

String DateTimeUtil::getDateString()
{
  String result;
  result += 2000 + ds3231.getYear();
  result += "-";
  result += ds3231.getMonth(century);
  result += "-";
  result += ds3231.getDate();
  return result;
}

void DateTimeUtil::saveCountdownTimestamp(long timestamp)
{
  uint8_t arr[5];
  for (int i = 0; i < 5; i++)
  {
    arr[i] = (timestamp & 0xff);
    timestamp >>= 8;
  }
  EEPROMTool.saveData(arr, 103, 5);
}

long DateTimeUtil::getCountdownTimestamp()
{
  long timestamp = 0;
  uint8_t *temp = EEPROMTool.loadData(103, 5); // 这里的103处理的不得当,后续优化,但是不影响实际功能
  for (int i = 0; i < 5; i++)
  {
    timestamp += temp[i] << (i * 8);
  }
  // 用完以后删除内存
  free(temp);
  return timestamp;
}
