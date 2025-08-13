#include "PCController.h"

void PCController::init()
{
    // 将引脚设置为输出模式
    pinMode(PIN_PC_POWER, OUTPUT);
    // 初始状态现在必须是【低电平】，这才是安全的非触发状态
    digitalWrite(PIN_PC_POWER, LOW); 
    Serial.print("PC Controller Initialized");
}

void PCController::trigger()
{
    Serial.print("Triggering PC power pulse...");
    // 1. 触发脉冲是【拉高】电平
    digitalWrite(PIN_PC_POWER, HIGH);

    // 2. 保持500毫秒
    delay(500);

    // 3. 结束脉冲是【拉低】电平，回到待机状态
    digitalWrite(PIN_PC_POWER, LOW);
    Serial.print("PC power pulse sent.");
}
