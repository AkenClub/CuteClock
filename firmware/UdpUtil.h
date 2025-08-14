#ifndef _UDP_UTIL_H_
#define _UDP_UTIL_H_

#include <WiFiUdp.h>
#include "JsonUtilUdp.h"
#include "UserConfig.h"

const static int localPort = UserUdpLocalPort;           // 监听本地UDP数据包端口
const static int remoteBroadcastPort = UserUdpBroadcastPort; // 远程广播端口

class UdpUtil
{

private:
    WiFiUDP _udp;
    boolean _connected;

public:
    boolean init();
    void broadcast();
};

#endif