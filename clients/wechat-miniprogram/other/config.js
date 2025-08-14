// 统一的客户端配置（不包含敏感信息）
// 请根据您的实际设备配置进行调整

const CONFIG = {
  // 默认设备端口（与固件 UserHttpServerPort 一致）
  defaultPort: 1229,

  // UDP 广播接收端口（与固件 UserUdpBroadcastPort 一致）
  udpBroadcastPort: 1998,

  // 可选：默认设备地址（如需固定IP，可在此指定）
  defaultDeviceAddress: "",

  // HTTP API 密钥（与固件 UserHttpApiKey 一致）。如为空，则不携带
  apiKey: "cute_clock_2025",
};
module.exports = { CONFIG };
