#ifndef PC_CONTROLLER_H
#define PC_CONTROLLER_H

#include <Arduino.h>

// 新增：定义用于控制电脑电源的引脚
#define PIN_PC_POWER D8

/**
 * @class PCController
 * @brief 电脑电源控制器
 * 负责初始化引脚和发送开关机脉冲信号
 */
class PCController
{
public:
    /**
     * @brief 初始化函数
     * 在 setup() 中调用，设置引脚模式和初始状态
     */
    void init();

    /**
     * @brief 触发函数
     * 发送一个低电平脉冲，模拟按下电源键
     */
    void trigger();
};

#endif
