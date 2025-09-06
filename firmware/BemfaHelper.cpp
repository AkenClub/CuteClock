#include "BemfaHelper.h"

// 全局PC控制器实例
extern PCController pcController;

BemfaHelper::BemfaHelper()
{
    isConnected = false;
    isPreviousConnected = false;
    lastHeartbeatTime = 0;
    lastConnectTime = 0;
    receiveBuffer = "";
    bufferIndex = 0;
    lastReceiveTime = 0;
}

BemfaHelper::~BemfaHelper()
{
    disconnectFromServer();
}

void BemfaHelper::init(const String &uid, 
                       const String &lightTopic, 
                       const String &brightnessTopic,
                       const String &sensorTopic,
                       const String &switchTopic)
{
    userUID = uid;
    
    // 直接使用用户配置的完整主题名（已包含设备类型后缀）
    this->lightTopic = lightTopic;        // 灯泡设备（应以002结尾）
    this->brightnessTopic = brightnessTopic; // 时钟亮度设备（应以002结尾）
    this->sensorTopic = sensorTopic;      // 传感器设备（应以004结尾）
    this->switchTopic = switchTopic;      // 开关设备（应以006结尾）
    
    Serial.println("巴法云初始化完成");
    Serial.println("灯光主题: " + this->lightTopic);
    Serial.println("亮度主题: " + this->brightnessTopic);
    Serial.println("传感器主题: " + this->sensorTopic);
    Serial.println("开关主题: " + this->switchTopic);
    
    // 验证主题名格式是否正确
    validateTopicName(this->lightTopic, "灯光", "002");
    validateTopicName(this->brightnessTopic, "时钟亮度", "002");
    validateTopicName(this->sensorTopic, "传感器", "004");
    validateTopicName(this->switchTopic, "开关", "006");
    
    // 初始化PC控制器
    pcController.init();
}

void BemfaHelper::run()
{
    // 检查WiFi连接状态
    if (WiFi.status() != WL_CONNECTED) {
        return;
    }
    
    // 检查TCP连接状态
    if (!tcpClient.connected()) {
        if (isPreviousConnected) {
            isPreviousConnected = false;
            isConnected = false;
            lastConnectTime = millis();
            Serial.println("巴法云TCP连接断开");
            tcpClient.stop();
        }
        else if (millis() - lastConnectTime > 5000) { // 5秒后重连
            connectToServer();
        }
    }
    else {
        if (!isPreviousConnected) {
            isPreviousConnected = true;
            isConnected = true;
            Serial.println("巴法云TCP连接成功");
            subscribeTopics();
            lastHeartbeatTime = millis();
        }
        
        // 处理接收的数据
        if (tcpClient.available()) {
            char c = tcpClient.read();
            receiveBuffer += c;
            bufferIndex++;
            lastReceiveTime = millis();
            
            // 防止缓冲区溢出
            if (bufferIndex >= BEMFA_MAX_PACKETSIZE - 1) {
                bufferIndex = BEMFA_MAX_PACKETSIZE - 2;
                lastReceiveTime = lastReceiveTime - 200;
            }
            lastHeartbeatTime = millis(); // 收到数据也算心跳
        }
        
        // 处理完整的数据包
        if ((receiveBuffer.length() >= 1) && (millis() - lastReceiveTime >= 200)) {
            processReceivedData();
            receiveBuffer = "";
            bufferIndex = 0;
        }
        
        // 发送心跳和上传传感器数据（合并为一次发送，参考巴法云示例）
        if (millis() - lastHeartbeatTime >= BEMFA_HEARTBEAT_INTERVAL) {
            sendHeartbeatWithData();
        }
    }
}

void BemfaHelper::connectToServer()
{
    if (tcpClient.connect(BEMFA_TCP_SERVER_ADDR, BEMFA_TCP_SERVER_PORT)) {
        Serial.print("连接到巴法云服务器: ");
        Serial.print(BEMFA_TCP_SERVER_ADDR);
        Serial.print(":");
        Serial.println(BEMFA_TCP_SERVER_PORT);
        
        tcpClient.setNoDelay(true);
        lastConnectTime = millis();
    }
    else {
        Serial.println("连接巴法云服务器失败");
        tcpClient.stop();
        isConnected = false;
        lastConnectTime = millis();
    }
}

void BemfaHelper::disconnectFromServer()
{
    if (tcpClient.connected()) {
        tcpClient.stop();
    }
    isConnected = false;
    isPreviousConnected = false;
}

void BemfaHelper::sendToServer(const String &message)
{
    if (!tcpClient.connected()) {
        Serial.println("巴法云客户端未连接");
        return;
    }
    
    tcpClient.print(message);
    Serial.println("[发送到巴法云]: " + message);
}

void BemfaHelper::subscribeTopics()
{
    // 订阅灯光控制主题、亮度控制主题和开关主题
    String subscribeCmd = "cmd=" + String(BEMFA_CMD_SUBSCRIBE) + 
                         "&uid=" + userUID + 
                         "&topic=" + lightTopic + "," + brightnessTopic + "," + switchTopic + "\r\n";
    sendToServer(subscribeCmd);
}

void BemfaHelper::processReceivedData()
{
    tcpClient.flush();
    Serial.println("[巴法云收到]: " + receiveBuffer);
    
    // 处理灯光控制消息
    if (receiveBuffer.indexOf("topic=" + lightTopic) > 0) {
        handleLightControl(receiveBuffer);
    }
    // 处理时钟亮度控制消息
    else if (receiveBuffer.indexOf("topic=" + brightnessTopic) > 0) {
        handleBrightnessControl(receiveBuffer);
    }
    // 处理开关控制消息（PC电源）
    else if (receiveBuffer.indexOf("topic=" + switchTopic) > 0) {
        handleSwitchControl(receiveBuffer);
    }
}

void BemfaHelper::handleLightControl(const String &message)
{
    if (message.indexOf("&msg=on") > 0) {
        Serial.println("巴法云: 打开灯光");
        LightUtil::setLightState(ROOM_LIGHT_ON);
        
        // 同步状态到小爱同学
        publishMessage(lightTopic + "/up", "on");
    }
    else if (message.indexOf("&msg=off") > 0) {
        Serial.println("巴法云: 关闭灯光");
        LightUtil::setLightState(ROOM_LIGHT_OFF);
        
        // 同步状态到小爱同学
        publishMessage(lightTopic + "/up", "off");
    }

    uploadSensorData();
}

void BemfaHelper::handleBrightnessControl(const String &message)
{
    // 在巴法云里面，设置亮度时候格式是 &msg=on#亮度值，所以这里需要判断是否包含 &msg=on#即可
    // 检查是否包含亮度设置 (格式: on#亮度值)
    int brightnessIndex = message.indexOf("&msg=on#");
    if (brightnessIndex >= 0) {
        // 提取亮度值
        String brightnessStr = message.substring(brightnessIndex + 8); // 跳过 "&msg=on#"
        int brightness = brightnessStr.toInt();
        
        Serial.println("巴法云: 设置时钟亮度 " + String(brightness));

        // 兼容小爱同学控制“把时钟调到最亮”的情况（这时小爱同学会传入100）
        if (brightness > GLOBAL_CLOCK_BRIGHTNESS_MAX) {
            brightness = GLOBAL_CLOCK_BRIGHTNESS_MAX;
        }
        
        // 验证亮度范围（参考点灯科技逻辑）
        if (brightness >= GLOBAL_CLOCK_BRIGHTNESS_LOW && brightness <= GLOBAL_CLOCK_BRIGHTNESS_MAX) {
            // 设置时钟亮度
            GLOBAL_CLOCK_BRIGHTNESS = brightness;
            
            // 同步状态到小爱同学
            publishMessage(brightnessTopic + "/up", "on#" + String(brightness));
            
            // 上传状态数据
            uploadSensorData();
            
            Serial.println("巴法云: 已设置时钟亮度 " + String(brightness));
        } else {
            Serial.println("巴法云: 非法时钟亮度 " + String(brightness));
            // 即使亮度非法，也要同步状态
            uploadSensorData();
        }
    }
}

void BemfaHelper::handleSwitchControl(const String &message)
{
    if (message.indexOf("&msg=on") > 0 || message.indexOf("&msg=off") > 0) {
        Serial.println("巴法云: 触发PC电源开关");
        pcController.trigger();
        
        // 反馈给小爱同学
        publishMessage(switchTopic + "/up", "triggered");

        uploadSensorData();
    }
}

void BemfaHelper::sendHeartbeatWithData()
{
    // 先发送心跳
    sendToServer("ping\r\n");
    
    // 然后上传传感器数据
    uploadSensorData();
    
    lastHeartbeatTime = millis();
}

void BemfaHelper::uploadSensorData()
{
    // 上传温度和灯光状态数据，官方支持的格式为#温度#湿度#开关#，
    // 我们只用到了温度这个数据，所以只需要保证 #温度# 开头，后面拼接自己定义的数据也可以。
    // 格式: #温度#开关_开关值#灯_状态值#时钟亮度_亮度值#
    String lightState = LightUtil::getLightState();
    String sensorData = "#" + String(GLOBAL_ROOM_TEMPERATURE) + 
                       "#灯_" + lightState + 
                       "#时钟亮度_" + String(GLOBAL_CLOCK_BRIGHTNESS) + "#";
    
    String uploadCmd = "cmd=" + String(BEMFA_CMD_PUBLISH) + 
                      "&uid=" + userUID + 
                      "&topic=" + sensorTopic + 
                      "&msg=" + sensorData + "\r\n";
    
    sendToServer(uploadCmd);
    // 注意：不更新 lastUploadTime，因为现在由心跳统一控制
}

void BemfaHelper::publishMessage(const String &topic, const String &message)
{
    String publishCmd = "cmd=" + String(BEMFA_CMD_PUBLISH) + 
                       "&uid=" + userUID + 
                       "&topic=" + topic + 
                       "&msg=" + message + "\r\n";
    sendToServer(publishCmd);
}

void BemfaHelper::subscribeTopic(const String &topic)
{
    String subscribeCmd = "cmd=" + String(BEMFA_CMD_SUBSCRIBE) + 
                         "&uid=" + userUID + 
                         "&topic=" + topic + "\r\n";
    sendToServer(subscribeCmd);
}

void BemfaHelper::setLightState(const String &state)
{
    Serial.println("巴法云设置灯光状态: " + state);
    LightUtil::setLightState(state);
    
    // 发布状态更新（使用/up后缀，只更新云端数据，不推送）
    publishMessage(lightTopic + "/up", state);
}

void BemfaHelper::triggerPCPower()
{
    Serial.println("巴法云触发PC电源");
    pcController.trigger();
    
    // 发布状态更新
    publishMessage(switchTopic + "/up", "triggered");
}

void BemfaHelper::validateTopicName(const String &topic, const String &deviceType, const String &expectedSuffix)
{
    // 检查主题名是否只包含字母和数字
    bool isValidFormat = true;
    for (int i = 0; i < topic.length(); i++) {
        char c = topic.charAt(i);
        if (!isalnum(c)) { // 如果不是字母或数字
            isValidFormat = false;
            break;
        }
    }
    
    if (!isValidFormat) {
        Serial.println("警告: " + deviceType + "主题名「" + topic + "」包含非法字符，只能包含字母和数字");
    }
    
    // 检查主题名后缀是否正确
    if (!topic.endsWith(expectedSuffix)) {
        Serial.println("警告: " + deviceType + "主题名应以" + expectedSuffix + "结尾，否则小爱同学无法识别为" + deviceType + "设备");
    }
    
    // 检查主题名长度是否合理
    if (topic.length() < 4) {
        Serial.println("警告: " + deviceType + "主题名「" + topic + "」过短，建议至少4个字符");
    }
    if (topic.length() > 20) {
        Serial.println("警告: " + deviceType + "主题名「" + topic + "」过长，建议不超过20个字符");
    }
}
