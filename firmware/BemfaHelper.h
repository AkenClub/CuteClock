#ifndef _BEMFA_HELPER_H_
#define _BEMFA_HELPER_H_

#include <ESP8266WiFi.h>
#include "GlobalConfigure.h"
#include "LightUtil.h"
#include "PCController.h"

// 巴法云服务器配置
#define BEMFA_TCP_SERVER_ADDR "bemfa.com"
#define BEMFA_TCP_SERVER_PORT 8344

// 巴法云命令类型
#define BEMFA_CMD_SUBSCRIBE 1         // 订阅消息
#define BEMFA_CMD_PUBLISH 2           // 发布消息

// 最大数据包大小
#define BEMFA_MAX_PACKETSIZE 512

// 心跳间隔（毫秒）- 参考巴法云示例，使用60秒间隔
#define BEMFA_HEARTBEAT_INTERVAL 60000

/**
 * @class BemfaHelper
 * @brief 巴法云帮助类，支持TCP协议连接和小爱同学控制
 * 实现与巴法云平台的通信，支持灯光控制、传感器数据上传、PC控制等功能
 */
class BemfaHelper
{
private:
    // TCP客户端
    WiFiClient tcpClient;
    
    // 连接状态
    bool isConnected;
    bool isPreviousConnected;
    
    // 时间戳
    unsigned long lastHeartbeatTime;
    unsigned long lastConnectTime;
    
    // 接收缓冲区
    String receiveBuffer;
    unsigned int bufferIndex;
    unsigned long lastReceiveTime;
    
    // 用户配置
    String userUID;
    String lightTopic;      // 灯泡控制主题 (后缀002)
    String brightnessTopic; // 时钟亮度控制主题 (后缀002)
    String sensorTopic;     // 传感器数据主题 (后缀004)  
    String switchTopic;     // 开关设备主题 (后缀006)
    
    // 内部方法
    void connectToServer();
    void disconnectFromServer();
    void sendToServer(const String &message);
    void processReceivedData();
    void sendHeartbeatWithData();
    void uploadSensorData();
    void handleLightControl(const String &message);
    void handleBrightnessControl(const String &message);
    void handleSwitchControl(const String &message);
    void subscribeTopics();
    void validateTopicName(const String &topic, const String &deviceType, const String &expectedSuffix);
    
public:
    BemfaHelper();
    ~BemfaHelper();
    
    /**
     * @brief 初始化巴法云连接
     * @param uid 用户私钥UID
     * @param lightTopic 灯光控制主题名（必须以002结尾）
     * @param brightnessTopic 时钟亮度控制主题名（必须以002结尾）
     * @param sensorTopic 传感器主题名（必须以004结尾）
     * @param switchTopic 开关主题名（必须以006结尾）
     */
    void init(const String &uid, 
              const String &lightTopic = "roomlight002", 
              const String &brightnessTopic = "clockbrightness002",
              const String &sensorTopic = "roomsensor004",
              const String &switchTopic = "pcswitch006");
    
    /**
     * @brief 主循环处理函数，需要在main loop中调用
     */
    void run();
    
    /**
     * @brief 发布消息到指定主题
     * @param topic 主题名
     * @param message 消息内容
     */
    void publishMessage(const String &topic, const String &message);
    
    /**
     * @brief 订阅指定主题
     * @param topic 主题名
     */
    void subscribeTopic(const String &topic);
    
    /**
     * @brief 设置灯光状态（小爱同学控制入口）
     * @param state "on"或"off"
     */
    void setLightState(const String &state);
    
    /**
     * @brief 触发PC开关（小爱同学控制入口）
     */
    void triggerPCPower();
};

#endif
