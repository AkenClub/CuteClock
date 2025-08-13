#include "UdpUtil.h"

boolean UdpUtil::init()
{
    Serial.println("Udp init");
    if (_udp.begin(localPort))
    {
        Serial.println("UDP initialized successfully");
        _connected = true;
    }
    else
    {
        Serial.println("UDP failed to connect");
        _connected = false;
    }
    return _connected;
}

void UdpUtil::broadcast()
{
    if (!_connected)
    {
        Serial.println("UDP not connected....");
        return;
    }
    Serial.println("--发送广播--");
    //向udp工具发送消息
    _udp.beginPacket("255.255.255.255", remoteBroadcastPort); //配置远端ip地址和端口
    _udp.write(JsonUtilUdp::getBroadcastJson().c_str());      //把数据写入发送缓冲区
    _udp.endPacket();                                         //发送数据
}
