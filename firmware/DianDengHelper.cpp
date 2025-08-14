#include "DianDengHelper.h"

#include <Blinker.h>

// 新建组件对象
BlinkerButton LightButton("btn-light");
BlinkerButton RefreshButton("btn-refresh");
BlinkerButton PcPowerButton("btn-pc");
BlinkerNumber TemperatureNum("num-temperature");
BlinkerSlider ClockSlider("ran-clock-slider");

PCController pcController;

// 电脑电源按钮的回调函数
void powerButtonCallback(const String &state)
{
    BLINKER_LOG("PC power button pressed, state: ", state);
    pcController.trigger(); // 调用我们封装好的trigger函数
    Blinker.print("已发送电脑开/关机指令");

    // 给APP一个反馈，让按键有响应
    PcPowerButton.print("on");
}

void weatherData(const String &data)
{
  BLINKER_LOG("weather: ", data);
  WeatherData tmp = JsonUtilWeatherApi::resolveWeather(data);
  SET_GLOBAL_OUTDOOR_WEATHER(tmp);
}

// 发送 APP 控件的状态
void sendAllStatus()
{
  LightButton.print(LightUtil::getLightState());
  ClockSlider.print(GLOBAL_CLOCK_BRIGHTNESS_NOW);
  TemperatureNum.print(GLOBAL_ROOM_TEMPERATURE);
}

// 处理时钟亮度
boolean handleBright(int brightLevel)
{
  BLINKER_LOG("将设置时钟亮度： ", brightLevel);
  Blinker.print("将设置时钟亮度：", brightLevel);

  if (brightLevel >= GLOBAL_CLOCK_BRIGHTNESS_LOW && brightLevel <= GLOBAL_CLOCK_BRIGHTNESS_MAX)
  {
    GLOBAL_CLOCK_BRIGHTNESS = brightLevel;
    ClockSlider.print(brightLevel);
    Blinker.print("已设置时钟亮度：", brightLevel);
    BLINKER_LOG("已设置时钟亮度：", brightLevel);

    return true;
  }
  Blinker.print("非法 时钟亮度:", brightLevel);
  BLINKER_LOG("非法 时钟亮度: ", brightLevel);
  return false;
}

// 灯状态处理
boolean handleLight(const String &state)
{
  BLINKER_LOG("将设置灯状态: ", state);
  Blinker.print("将设置灯状态", state);

  if (state != BLINKER_CMD_ON && state != BLINKER_CMD_OFF)
  {
    BLINKER_LOG("灯状态值非法: ", state);
    Blinker.print("灯状态值非法", state);
    return false;
  }

  LightUtil::setLightState(state);

  LightButton.print(state);

  BLINKER_LOG("已设置灯的状态为: ", state);
  Blinker.print("已设置灯的状态为：", state);

  return true;
}

// APP 按下开关灯控件 回调
void lightBtnCallback(const String &state)
{
  handleLight(state);
}

// APP 滑动时钟亮度控件 回调
void clockSliderCallback(int32_t value)
{
  handleBright(value);
}

// APP 按下刷新控件 回调
void refreshBtnCallback(const String &state)
{
  Serial.println("refreshBtnCallback");
  sendAllStatus();
}

// 如果未绑定的组件被触发，则会执行其中内容
void dataRead(const String &data)
{
  BLINKER_LOG("Blinker 未知命令 readString: ");
  BLINKER_LOG(data);
}

void dataStorage()
{
  Blinker.dataStorage("chart-temperature", GLOBAL_ROOM_TEMPERATURE);
}

// 小爱同学设置灯开关 回调
void miotPowerState(const String &state)
{
  BLINKER_LOG("小爱 需要设置灯开关: ", state);

  // 提前返回响应给小爱，避免提示无响应
  BlinkerMIOT.powerState(state);
  BlinkerMIOT.print();

  boolean isHandled = handleLight(state);
  if (isHandled)
  {
    BLINKER_LOG("小爱 已设置灯的状态为：", state);
  }
  else
  {
    BLINKER_LOG("小爱 设置灯开关 失败");
  }
}

// 小爱同学接收控制灯模式的回调
void miotMode(uint8_t mode)
{
    BLINKER_LOG("小爱设置灯模式: ", mode);

    // 让对小爱回应提前，避免小爱回复设备无响应
    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();

    if (mode == BLINKER_CMD_MIOT_DAY) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COLOR) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_TV) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_READING) {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER) {
        // 设置电脑模式，实际上是执行开关电脑操作
        powerButtonCallback(BLINKER_CMD_ON);
        BLINKER_LOG("小爱执行PC开关");
    }
}

void miotBright(const String &bright)
{
  BLINKER_LOG("小爱 需要设置时钟亮度: ", bright);
  int brightLevel = bright.toInt();

  // 让对小爱回应提前，避免小爱回复设备无响应
  BlinkerMIOT.brightness(brightLevel);
  BlinkerMIOT.print();

  boolean isHandled = handleBright(brightLevel);
  if (isHandled)
  {
    BLINKER_LOG("小爱 已设置时钟亮度 成功：", bright);
  }
  else
  {
    BLINKER_LOG("小爱 设置时钟亮度 失败");
  }
}

void miotQuery(int32_t queryCode)
{
  BLINKER_LOG("MIOT Query codes: ", queryCode);

  switch (queryCode)
  {
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER:
      BLINKER_LOG("MIOT Query POWER");
      BlinkerMIOT.powerState(LightUtil::getLightState());
      BlinkerMIOT.print();
      break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER:
      BLINKER_LOG("MIOT Query BRIGHTNESS");
      BlinkerMIOT.brightness(GLOBAL_CLOCK_BRIGHTNESS_NOW);
      BlinkerMIOT.print();
      break;
    default:
      BlinkerMIOT.powerState(LightUtil::getLightState());
      BlinkerMIOT.brightness(GLOBAL_CLOCK_BRIGHTNESS_NOW);
      BlinkerMIOT.temp(GLOBAL_ROOM_TEMPERATURE);
      BlinkerMIOT.print();
      break;
  }
}

void heartbeat() //自定义blinker心跳包数据执行函数---------------------
{
  sendAllStatus();
  Serial.println("点灯 心跳~~");
  Blinker.print("点灯 心跳~~");
}

Times DianDengHelper::getTime()
{
  int8_t sec = Blinker.second();
  int8_t min = Blinker.minute();
  int8_t hour = Blinker.hour();

  if (sec < 0 || min < 0 || hour < 0)
  {
    // Serial.println("***获取时间失败***");
    return times;
  }

  times.h = hour;
  times.m = min;
  times.s = sec;
  return times;
}

Dates DianDengHelper::getDates()
{
  dates.y = Blinker.year();
  dates.m = Blinker.month();
  dates.d = Blinker.mday();
  return dates;
}

void DianDengHelper::init()
{
#if defined(BLINKER_PRINT)
  BLINKER_DEBUG.stream(BLINKER_PRINT);
#endif
  // 初始化blinker
  Blinker.begin(auth, ssid, pswd);
  // Blinker.begin(auth);
  Blinker.setTimezone(8.0);
  Serial.println("----点灯 connecting---");
  // 有时在APP连接上时出现 null，需要调试时候再打开
  // Blinker.attachData(dataRead);
  Blinker.attachHeartbeat(heartbeat); //自定义状态返回的回调函数,每30s-60会发送一次心跳包
  Blinker.attachDataStorage(dataStorage);
  Blinker.attachWeather(weatherData);

  LightButton.attach(lightBtnCallback);
  ClockSlider.attach(clockSliderCallback);
  RefreshButton.attach(refreshBtnCallback);

  // 因为小爱控制 8266 时候只能设置为某一种设备，所以这里就只用灯设备
  // 控制灯开关
  BlinkerMIOT.attachPowerState(miotPowerState);
  // 灵活使用灯设置某种模式来实现某种功能，例如这里设置灯亮度为电脑模式来实际调用开关电脑的逻辑，结合小爱自定义训练，训练“打开电脑”执行设置这个灯的亮度模式的电脑模式来实现真正打开电脑
  BlinkerMIOT.attachMode(miotMode);
  BlinkerMIOT.attachBrightness(miotBright);
  BlinkerMIOT.attachQuery(miotQuery);

  // 初始化电脑控制器，并绑定按钮回调
  pcController.init();
  PcPowerButton.attach(powerButtonCallback);
}

void DianDengHelper::run()
{
  Blinker.run();
}

boolean DianDengHelper::isNtpCorrectly()
{
  return Blinker.hour() != -1;
}

int DianDengHelper::getHour()
{
  return Blinker.hour();
}

int DianDengHelper::getMinute()
{
  return Blinker.minute();
}

uint32 DianDengHelper::getTimestamp()
{
  return Blinker.time();
}

int DianDengHelper::getSecond()
{
  return Blinker.second();
}

void DianDengHelper::getWeather()
{
  Serial.println("获取天气");
  Blinker.weather(CITY_CODE);
}
