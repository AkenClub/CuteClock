#include "Displayer.h"

static const uint8_t Small_font_0[] = {0xFE, 0x82, 0x82, 0xFE};
static const uint8_t Small_font_0_len[] = {4, 0xFE, 0x82, 0x82, 0xFE};
static const uint8_t Small_font_1[] = {0x00, 0x84, 0xFE, 0x80};
static const uint8_t Small_font_1_len[] = {4, 0x00, 0x84, 0xFE, 0x80};
static const uint8_t Small_font_2[] = {0xF2, 0x92, 0x92, 0x9E};
static const uint8_t Small_font_2_len[] = {4, 0xF2, 0x92, 0x92, 0x9E};
static const uint8_t Small_font_3[] = {0x92, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_3_len[] = {4, 0x92, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_4[] = {0x1E, 0x10, 0x10, 0xFE};
static const uint8_t Small_font_4_len[] = {4, 0x1E, 0x10, 0x10, 0xFE};
static const uint8_t Small_font_5[] = {0x9E, 0x92, 0x92, 0xF2};
static const uint8_t Small_font_5_len[] = {4, 0x9E, 0x92, 0x92, 0xF2};
static const uint8_t Small_font_6[] = {0xFE, 0x92, 0x92, 0xF2};
static const uint8_t Small_font_6_len[] = {4, 0xFE, 0x92, 0x92, 0xF2};
static const uint8_t Small_font_7[] = {0x02, 0x02, 0x02, 0xFE};
static const uint8_t Small_font_7_len[] = {4, 0x02, 0x02, 0x02, 0xFE};
static const uint8_t Small_font_8[] = {0xFE, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_8_len[] = {4, 0xFE, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_9[] = {0x9E, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_9_len[] = {4, 0x9E, 0x92, 0x92, 0xFE};
static const uint8_t Small_font_10_on[] = {0x28};
static const uint8_t Small_font_11_off[] = {0x00};
// 日期中间分隔线
static const uint8_t Small_font_12_date_separate_len[] = {2, 0x10, 0x10};
static const uint8_t Small_font_12_date_separate[] = {0x10, 0x10};

// 小狗精灵图标
static const uint8_t icon_dog[] = {0x8C, 0x4C, 0xFE, 0x30, 0xB0, 0x70, 0xF0, 0x08, 0x0C, 0x0C, 0xFE, 0x30, 0x30, 0x30, 0xF8, 0x00};

// WIFI
static const uint8_t icon_wifi[] = {0x04, 0x06, 0x13, 0xDB, 0xDB, 0x13, 0x06, 0x04, 0x00, 0x70, 0x18, 0x7d, 0xb6, 0x3c, 0x3c, 0xb6, 0x7d, 0x18, 0x70};

// 爱心
static const uint8_t icon_love[] = {7, 0x1C, 0x3E, 0x7E, 0xFC, 0x7E, 0x3E, 0x1C};

// I
static const uint8_t icon_I[] = {3, 0x82, 0xFE, 0x82};

// Y
static const uint8_t icon_Y[] = {5, 0x7E, 0x80, 0x80, 0x80, 0x7E};

// 温度单位符号 {6,6,112,136,136,136,0,0
static const uint8_t icon_temperature_unit_len[] = {6, 0x06, 0x06, 0x70, 0x88, 0x88, 0x88};

// 点
static const uint8_t icon_dot_len[] = {1, 0x80};

// 天气 - 晴 - 太阳
static const uint8_t icon_weather_sun[] = {8, 0x24, 0x00, 0x99, 0x24, 0x24, 0x99, 0x00, 0x24};

// 天气 - 晴 - 月亮
static const uint8_t icon_weather_moon[] = {8, 0x38, 0x7C, 0xE2, 0xC0, 0xC4, 0x4E, 0x24, 0x00};

// 天气 - 阴 多云
static const uint8_t icon_weather_cloud[] = {8, 0x18, 0x24, 0x24, 0x22, 0x22, 0x26, 0x24, 0x18};

// 天气 - 雨
static const uint8_t icon_weather_rain[] = {8, 0x8C, 0x52, 0x12, 0x91, 0x51, 0x13, 0x92, 0x4C};

// 天气 - 风
static const uint8_t icon_weather_wind[] = {8, 0x44, 0x44, 0x44, 0x45, 0x55, 0x56, 0x50, 0x60};

// 天气 - 未知
static const uint8_t icon_weather_unknown[] = {8, 0x3C, 0x42, 0xA5, 0x91, 0x91, 0xA5, 0x42, 0x3C};

// 日历
static const uint8_t icon_calendar[] = {8, 0x7C, 0x82, 0x93, 0xA2, 0x92, 0x8B, 0x82, 0x7C};

// 室内
static const uint8_t icon_home[] = {8, 0x08, 0xFC, 0x8E, 0xE3, 0xE3, 0x8E, 0xFC, 0x08};

static const uint8_t *bitmap_data[] = {
    Small_font_0,
    Small_font_1,
    Small_font_2,
    Small_font_3,
    Small_font_4,
    Small_font_5,
    Small_font_6,
    Small_font_7,
    Small_font_8,
    Small_font_9,
    Small_font_10_on,
    Small_font_11_off,
    Small_font_12_date_separate};

void Displayer::init()
{
    mx.begin();
    P.begin(2);

    P.setZone(0, 0, 2);
    P.setZone(1, 3, 3);

    P.addChar(0, '0', Small_font_0_len);
    P.addChar(0, '1', Small_font_1_len);
    P.addChar(0, '2', Small_font_2_len);
    P.addChar(0, '3', Small_font_3_len);
    P.addChar(0, '4', Small_font_4_len);
    P.addChar(0, '5', Small_font_5_len);
    P.addChar(0, '6', Small_font_6_len);
    P.addChar(0, '7', Small_font_7_len);
    P.addChar(0, '8', Small_font_8_len);
    P.addChar(0, '9', Small_font_9_len);
    P.addChar(0, '-', Small_font_12_date_separate_len);
    P.addChar(0, '*', icon_temperature_unit_len);
    P.addChar(0, '.', icon_dot_len);

    P.addChar(0, '&', icon_love);
    P.addChar(0, 'I', icon_I);
    P.addChar(0, 'Y', icon_Y);

    P.addChar(1, 'S', icon_weather_sun);
    P.addChar(1, 'M', icon_weather_moon);
    P.addChar(1, 'R', icon_weather_rain);
    P.addChar(1, 'W', icon_weather_wind);
    P.addChar(1, 'C', icon_weather_cloud);
    P.addChar(1, 'U', icon_weather_unknown);

    // 日历
    P.addChar(1, '#', icon_calendar);
    // 房子 室温
    P.addChar(1, '^', icon_home);

    P.setSpriteData(1, icon_dog, 8, 2, icon_dog, 8, 2);
}

void Displayer::run()
{
    P.displayAnimate();
    if (P.getZoneStatus(1))
    {
        P.displayZoneText(1, "", PA_CENTER, 100, 0, PA_SPRITE, PA_SPRITE);
    }
}

void Displayer::display_bitmap(int abscissa, int width, int bitmap_number)
{
    display_bitmap(abscissa, width, const_cast<uint8_t *>(bitmap_data[bitmap_number]));
}

void Displayer::display_bitmap(int abscissa, int width, uint8_t *bitmap)
{
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::OFF);
    mx.setBuffer(abscissa, width, bitmap);
    mx.control(MD_MAX72XX::UPDATE, MD_MAX72XX::ON);
}

void Displayer::clear()
{
    mx.clear();
}

void Displayer::showTime(Times times)
{
    if (_second_variable == times.s)
    {
        // Serial.println("时间不变，则不需更新");
        return;
    }

    _hour_variable = times.h;
    _minute_variable = times.m;
    _second_variable = times.s;

    Serial.print("更新时间：");
    Serial.println(_second_variable);

    display_bitmap(21, 4, _hour_variable / 10);
    display_bitmap(16, 4, _hour_variable % 10);
    if (_second_variable % 2 == 0)
    {
        display_bitmap(11, 1, 10);
    }
    else
    {
        display_bitmap(11, 1, 11);
    }
    display_bitmap(9, 4, _minute_variable / 10);
    display_bitmap(4, 4, _minute_variable % 10);

    if (_minute_variable == 0 && _second_variable == 0)
    {
        // 只在整点时候才调整亮度，其余时间接受远程调控。
        checkClockBrightnessByHour();
    }
}

void Displayer::showDate(Dates dates)
{
    if (_day_variable == dates.d)
    {
        // 日期没有变化，则不用刷新屏幕
        return;
    }
    Serial.println("更新日期：");
    _day_variable = dates.d;

    _displayTmp = String(dates.m);
    _displayTmp += "-";
    _displayTmp += dates.d;
    showNormalText(0, _displayTmp);

    showContent(1, "#");
}

void Displayer::showTemperature(float temperature)
{
    if (_temperature_variable == temperature || temperature == -9999.00)
    {
        // 温度值不变，不更新屏幕
        return;
    }
    Serial.print("更新室温：");
    Serial.println(temperature);
    _temperature_variable = temperature;
    SET_GLOBAL_ROOM_TEMPERATURE(temperature);

    showNormalText(0, String(temperature));

    showContent(1, "^");
}

void Displayer::showOutdoorWeather(WeatherData weatherData)
{

    if (_temperature_door_variable == weatherData.temperature)
    {
        return;
    }

    _temperature_door_variable = weatherData.temperature;

    Serial.println(weatherData.text);
    Serial.println(weatherData.temperature);

    if (weatherData.text.indexOf("晴") != -1)
    {
        if (_hour_variable >= 19)
        {
            // 月亮
            showNormalText(1, "M");
        }
        else
        {
            // 太阳
            showNormalText(1, "S");
        }
    }
    else if (weatherData.text.indexOf("云") != -1 || weatherData.text.indexOf("阴") != -1)
    {
        showNormalText(1, "C");
    }
    else if (weatherData.text.indexOf("风") != -1)
    {
        showNormalText(1, "W");
    }
    else if (weatherData.text.indexOf("雨") != -1)
    {
        showNormalText(1, "R");
    }
    else
    {
        // 未知
        showNormalText(1, "U");
    }

    strcpy(_displayDataTmpZone0, weatherData.temperature.c_str());
    showContent(0, _displayDataTmpZone0);
}

void Displayer::showContent(int zone, const char *content)
{
    P.displayZoneText(zone, content, PA_CENTER, 50, 3800, PA_SCROLL_UP, PA_SCROLL_UP);
}

void Displayer::showNormalText(int zone, String text)
{
    if (_normal_text_variable == 0)
    {
        _normal_text_variable = 1;
        _displayTmp = text;
        strcpy(_displayDataTmp, _displayTmp.c_str());
        showContent(zone, _displayDataTmp);
    }
}

void Displayer::resetDateTemperature()
{
    _day_variable = 99;
    _temperature_variable = 0.0f;
    _normal_text_variable = 0;
    _temperature_door_variable = "";
}

void Displayer::showWifi()
{
    display_bitmap(23, 19, const_cast<uint8_t *>(icon_wifi));
}

/**
 * @brief 设置时钟显示亮度
 * @param value 要设置的亮度值
 * 
 * 这个函数用于设置时钟的显示亮度，支持以下场景：
 * 
 * 1. 定时自动调整亮度调用
 * 2. 用户通过Web界面手动调节
 * 3. 系统初始化时设置默认亮度
 * 
 * 函数会：
 * - 验证亮度值是否在有效范围内
 * - 同时设置两个显示区域的亮度
 * - 更新全局亮度变量
 * - 输出调试信息到串口
 * 
 * 注意：此函数会同步更新全局变量，确保亮度设置的一致性
 */
void Displayer::setClockBrightness(int value)
{
    if (value <= GLOBAL_CLOCK_BRIGHTNESS_MAX && value >= GLOBAL_CLOCK_BRIGHTNESS_LOW)
    {
        P.setIntensity(0, value);
        P.setIntensity(1, value);
        // 设置全局的时钟亮度值
        SET_GLOBAL_CLOCK_BRIGHTNESS(value);
        Serial.print("设置全局的时钟亮度值:");
        Serial.println(value);
    }
    else
    {
        Serial.print("设置的时钟亮度超过范围：");
        Serial.println(value);
    }
}

void Displayer::checkClockBrightnessByHour(int hour)
{
    _hour_variable = hour;
    checkClockBrightnessByHour();
}

/**
 * @brief 根据当前小时自动调整时钟亮度
 * 
 * 定时自动调整亮度功能说明：
 * 
 * 根据一天中的不同时间段自动调整时钟显示亮度，提供更好的用户体验：
 * 
 * 🌅 白天时段 (7:00-19:00)：
 *    - 亮度设置为 4（较亮）
 *    - 适合白天光线充足的环境
 * 
 * 🌆 傍晚时段 (19:00-23:00)：
 *    - 亮度设置为 2（中等）
 *    - 适合傍晚光线较暗的环境
 * 
 * 🌙 夜间时段 (23:00-7:00)：
 *    - 亮度设置为最低值（GLOBAL_CLOCK_BRIGHTNESS_LOW）
 *    - 避免夜间过亮影响休息
 * 
 * 调用时机：
 * - 系统启动时（NTP时间同步成功后）
 * - 离线模式下使用DS3231时间
 * - 可通过Web界面手动调节覆盖自动设置
 * 
 * 注意：手动调节的亮度会保存在全局变量中，下次自动调节时会检测并应用
 * 
 * 配置控制：
 * - 可通过 UserClockAutoBrightness 配置项启用/禁用此功能
 * - 禁用后，亮度将保持用户手动设置的值
 */
void Displayer::checkClockBrightnessByHour()
{
    // 检查是否启用自动亮度调整
    #ifndef UserClockAutoBrightness
        #define UserClockAutoBrightness true
    #endif
    
    if (!UserClockAutoBrightness) {
        Serial.println("自动亮度调整功能已禁用");
        return;
    }
    
    int value;
    if (_hour_variable >= 7 && _hour_variable <= 19)
    {
        value = 4;
    }
    else if (_hour_variable > 19 && _hour_variable < 23)
    {
        value = 2;
    }
    else
    {
        value = GLOBAL_CLOCK_BRIGHTNESS_LOW;
    }

    setClockBrightness(value);
}
