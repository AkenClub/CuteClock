#ifndef _WIFI_SERVER_H_
#define _WIFI_SERVER_H_

#include <ESP8266WebServer.h> //  ESP8266WebServer库
#include "JsonWifiServer.h"
#include "GlobalConfigure.h"
#include "LightUtil.h"
#include "PCController.h"
#include "UserConfig.h"
#include "WebControl.h"

ESP8266WebServer _wifiServer(WIFI_SERVER_PORT); // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）
PCController _pcController; // 电脑控制器实例

void _sendSuccessResponse()
{
    String resp = JsonWifiServer::getSuccessResponse();
    _wifiServer.send(200, "application/json; charset=utf-8", resp);
}

void _sendFailResponse(String errMsg, int statusCode = 400)
{
    String resp = JsonWifiServer::getFailResponse(errMsg);
    _wifiServer.send(statusCode, "application/json; charset=utf-8", resp);
}

// 验证API密钥
bool _checkApiKey()
{
    // 如果用户配置的密钥为空字符串，直接放行（允许无密钥）
    String expectedKey = String(UserHttpApiKey);
    expectedKey.trim();
    if (expectedKey.length() == 0)
    {
        return true;
    }

    // 检查是否存在X-API-Key请求头（兼容大小写）
    bool hasUpper = _wifiServer.hasHeader("X-API-Key");
    bool hasLower = _wifiServer.hasHeader("x-api-key");
    if (!hasUpper && !hasLower)
    {
        Serial.println("缺少API密钥请求头");
        _sendFailResponse("缺少API密钥", 401);
        return false;
    }

    // 验证密钥是否正确
    String receivedKey = hasUpper ? _wifiServer.header("X-API-Key") : _wifiServer.header("x-api-key");
    receivedKey.trim();
    if (receivedKey != expectedKey)
    {
        Serial.println("API密钥验证失败");
        _sendFailResponse("API密钥无效", 401);
        return false;
    }
    
    Serial.println("API密钥验证成功");
    return true;
}

void _handleRoot()
{
    _wifiServer.send_P(200, "text/html; charset=utf-8", WEB_CONTROL_HTML);
}

void _handleNotFound()
{
    _wifiServer.send(404, "text/plain; charset=utf-8", "404: Not found");
}

void _getAllStatus()
{
    Serial.println("获取设备全部状态");
    String tmp = JsonWifiServer::getAllStatusJson(LightUtil::getLightState() == "on", GLOBAL_CLOCK_BRIGHTNESS_NOW, GLOBAL_ROOM_TEMPERATURE);
    _wifiServer.send(200, "application/json; charset=utf-8", tmp); // NodeMCU将调用此函数。
    Serial.println("回送设备全部状态");
}

void _setRoomLightStatus()
{
    Serial.println("设置房间灯状态");
    
    // 首先验证API密钥
    if (!_checkApiKey())
    {
        return; // 验证失败，已在_checkApiKey中发送错误响应
    }
    
    if (_wifiServer.hasArg("plain") == false)
    {
        Serial.println("设置灯状态接口内容为空");
        _sendFailResponse("接口内容为空", 400);
        return;
    }
    String status = JsonWifiServer::resolveSetRoomLightOder(_wifiServer.arg("plain"));

    if (status == "-1" || (status != "on" && status != "off"))
    {

        Serial.print("灯状态解析失败或设置值非法:");
        Serial.println(status);
        _sendFailResponse("灯状态解析失败或设置值非法", 400);
        return;
    }

    Serial.print("设置灯状态：");
    Serial.println(status);
    LightUtil::setLightState(status);

    _sendSuccessResponse();
    Serial.println("回送灯状态");
}

void _setClockStatus()
{
    Serial.println("设置时钟状态");
    
    // 首先验证API密钥
    if (!_checkApiKey())
    {
        return; // 验证失败，已在_checkApiKey中发送错误响应
    }
    
    if (_wifiServer.hasArg("plain") == false)
    {
        Serial.println("设置时钟状态接口内容为空");
        _sendFailResponse("接口内容为空", 400);
        return;
    }
    int level = JsonWifiServer::resolveSetClockBrightnessOder(_wifiServer.arg("plain"));
    if (level < 0 || level > 15)
    {
        Serial.println("时钟状态解析失败或设置值非法");
        _sendFailResponse("时钟状态解析失败或设置值非法", 400);
        return;
    }

    Serial.print("设置时钟亮度状态：");
    Serial.println(level);
    GLOBAL_CLOCK_BRIGHTNESS = level;
    _sendSuccessResponse();
    Serial.println("回送时钟状态");
}

void _setPcPower()
{
    Serial.println("控制电脑电源");
    
    // 首先验证API密钥
    if (!_checkApiKey())
    {
        return; // 验证失败，已在_checkApiKey中发送错误响应
    }
    
    Serial.println("触发电脑电源开关");
    _pcController.trigger();
    
    _sendSuccessResponse();
    Serial.println("电脑电源控制指令已发送");
}

void wifiServerInit()
{
    _pcController.init();                                   // 初始化电脑控制器
    _wifiServer.begin();                                    // 启动网站服务

    // 收集自定义请求头，确保后续可通过 hasHeader/header 读取
    const char *apiHeaderKeys[] = {"X-API-Key", "x-api-key"};
    const size_t apiHeaderKeysCount = sizeof(apiHeaderKeys) / sizeof(apiHeaderKeys[0]);
    _wifiServer.collectHeaders(apiHeaderKeys, apiHeaderKeysCount);
    _wifiServer.on("/", _handleRoot);                       // 设置服务器根目录即'/'的函数'handleRoot'
    _wifiServer.on("/all-status", HTTP_GET, _getAllStatus); //
    _wifiServer.on("/room-light", HTTP_POST, _setRoomLightStatus);
    _wifiServer.on("/clock", HTTP_POST, _setClockStatus);
    _wifiServer.on("/pc-power", HTTP_POST, _setPcPower);   // 电脑电源控制接口
    _wifiServer.onNotFound(_handleNotFound); // 设置处理404情况的函数'handleNotFound'

    Serial.println("HTTP esp8266_server started"); //  告知用户ESP8266网络服务功能已经启动
}

void wifiServerRun()
{
    _wifiServer.handleClient(); // 处理http服务器访问
}

#endif
