#include "LightUtil.h"

void LightUtil::initLightState()
{
    // 设置连着灯的引脚模式
    pinMode(ROOM_LIGHT, OUTPUT);

    // setLightState(ROOM_LIGHT_OFF);
    setLightState(ROOM_LIGHT_OFF);

    Serial.print("电灯工具初始化，目前值为：");
    Serial.println(getLightState());
}

// 设置灯状态
void LightUtil::setLightState(const String &state)
{
    digitalWrite(ROOM_LIGHT, state == ROOM_LIGHT_ON ? LOW : HIGH);
    GLOBAL_ROOM_LIGHT_STATUS = state;
    Serial.println("电灯工具设置状态值为：" + GLOBAL_ROOM_LIGHT_STATUS);
}

// 获取灯状态值
String LightUtil::getLightState()
{
    return GLOBAL_ROOM_LIGHT_STATUS;
}
