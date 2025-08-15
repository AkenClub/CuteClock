#include "JsonWifiServer.h"
#include "UserConfig.h"

String JsonWifiServer::getAllStatusJson(boolean enable, int bright, float temperature)
{

  StaticJsonDocument<768> doc;

  doc["errCode"] = 0;
  doc["errMsg"] = "获取成功";

  JsonObject data = doc.createNestedObject("data");
  data["id"] = UserDeviceId;
  data["order"] = "RESPONSE_ALL_STATUS";

  JsonArray data_items = data.createNestedArray("items");

  JsonObject data_items_0 = data_items.createNestedObject();
  data_items_0["type"] = "light";
  data_items_0["id"] = "light-bedroom-01";
  data_items_0["mark"] = UserLightDeviceName;
  data_items_0["data"]["enable"] = enable;

  JsonObject data_items_1 = data_items.createNestedObject();
  data_items_1["type"] = "clock";
  data_items_1["id"] = "clock-living_room-01";
  data_items_1["mark"] = UserClockDeviceName;

  JsonObject data_items_1_data = data_items_1.createNestedObject("data");
  data_items_1_data["enable"] = true;
  data_items_1_data["brightness"] = bright;
  data_items_1_data["mode"] = 1;
  data_items_1_data["brightnessMin"] = GLOBAL_CLOCK_BRIGHTNESS_LOW;
  data_items_1_data["brightnessMax"] = GLOBAL_CLOCK_BRIGHTNESS_MAX;

  JsonObject data_items_2 = data_items.createNestedObject();
  data_items_2["type"] = "pc";
  data_items_2["id"] = "pc-desktop-01";
  data_items_2["mark"] = UserPcDeviceName;

  JsonObject data_items_2_data = data_items_2.createNestedObject("data");
  data_items_2_data["enable"] = true;
  data_items_2_data["description"] = "电源开关控制";

  JsonObject data_items_3 = data_items.createNestedObject();
  data_items_3["type"] = "temperature";
  data_items_3["id"] = "temperature-sensor-01";
  data_items_3["mark"] = "室内温度";

  JsonObject data_items_3_data = data_items_3.createNestedObject("data");
  data_items_3_data["value"] = temperature;
  data_items_3_data["unit"] = "°C";

  String output;
  serializeJson(doc, output);
  return output;
}

/**
 * @brief
{
  "id": "light-bedroom-01",
  "order": {
    "type": "enable",
    "value": false
  }
}
 *
 * @param input
 * @return boolean
 */
String JsonWifiServer::resolveSetRoomLightOder(String input)
{
  Serial.println(input);
  StaticJsonDocument<128> doc;

  DeserializationError error = deserializeJson(doc, input);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return "-1";
  }
  return doc["order"]["value"];
}

/**
 * @brief
 *
{
  "id": "clock-living_room-01",
  "order": {
    "type": "brightness",
    "value": 1
  }
}
 *
 * @param input
 * @return int
 */
int JsonWifiServer::resolveSetClockBrightnessOder(String input)
{
  Serial.println(input);
  StaticJsonDocument<128> doc;
  DeserializationError error = deserializeJson(doc, input);

  if (error)
  {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return -1;
  }
  return doc["order"]["value"];
}

/**
 * @brief
{
  "errCode": 0,
  "errMsg": "成功"
}
 *
 * @return String
 */
String JsonWifiServer::getSuccessResponse()
{
  StaticJsonDocument<48> doc;

  doc["errCode"] = 0;
  doc["errMsg"] = "成功";

  String output;
  serializeJson(doc, output);
  return output;
}

/**
 * @brief
{
  "errCode": -1,
  "errMsg": "失败"
}
 *
 * @return String
 */
String JsonWifiServer::getFailResponse(String errMsg)
{
  StaticJsonDocument<128> doc;

  doc["errCode"] = -1;
  doc["errMsg"] = errMsg;

  String output;
  serializeJson(doc, output);
  return output;
}
