#include "JsonUtilUdp.h"
#include "UserConfig.h"

String JsonUtilUdp::getBroadcastJson()
{
    StaticJsonDocument<192> doc;

    JsonObject udp = doc.createNestedObject("udp");
    udp["type"] = "SERVER-BROCAST";

    JsonObject udp_data_device = udp["data"].createNestedObject("device");
    udp_data_device["type"] = "esp8266";

    JsonObject udp_data_device_data = udp_data_device.createNestedObject("data");
    udp_data_device_data["id"] = UserDeviceId;
    udp_data_device_data["mac"] = WiFi.macAddress();
    udp_data_device_data["mark"] = UserDeviceRoomName;
    udp_data_device_data["port"] = WIFI_SERVER_PORT;

    String output;
    serializeJson(doc, output);
    return output;
}
