# 🚀 配置文件设置

在编译和上传代码之前，您需要根据自己的实际情况配置项目。

## 第一步：创建配置文件

1. 进入 `cute-clock` 目录
2. 复制 `UserConfig.example.h` 为 `UserConfig.h`
   ```bash
   cp UserConfig.example.h UserConfig.h
   ```
3. 编辑 `UserConfig.h` 文件，按照以下指南配置各项参数

## 📝 配置项详细说明

### 🔐 必须配置的敏感信息

#### WiFi配置
> 仅支持 2.4GHz WiFi
```cpp
#define UserWifiSSID "Your_WiFi_Name"          // 修改为您的WiFi名称
#define UserWifiPassword "Your_WiFi_Password"    // 修改为您的WiFi密码
```

**说明：** 设备需要连接WiFi来获取网络时间和天气信息。

#### 点灯科技配置
**获取方法：**
1. 下载并安装[点灯科技App](https://diandeng.tech/dev)
2. 打开App，点击右上角的"+"号
3. 选择"添加设备" > "独立设备"（免费用户可申请1个设备）
4. 选择"Arduino" > "WiFi接入"
5. 复制生成的Secret Key

详细教程：[点灯科技官方文档](https://diandeng.tech/doc/getting-start-8266#%E5%9C%A8app%E4%B8%AD%E6%B7%BB%E5%8A%A0%E8%AE%BE%E5%A4%87%EF%BC%8C%E8%8E%B7%E5%8F%96secret-key)
```cpp
#define UserBlinkerAuthKey "your_auth_key"         // 修改为您的点灯科技设备密钥
```

### 🌍 地理位置配置

#### 城市代码
```cpp
#define UserWeatherCityCode 440123                 // 修改为您所在城市的代码
```

**获取方法：**
- 自行查询，搜索 "国家行政区编码"

<details>
<summary><b>以下配置项可以不用修改：</b></summary>

### 🔧 网络服务配置（可选）

```cpp
#define UserHttpServerPort 1229                    // HTTP服务器监听端口
#define UserUdpLocalPort 2506                      // 本地UDP监听端口
#define UserUdpBroadcastPort 1998                  // 远程广播端口
```

**说明：** 如果这些端口在您的网络中被占用，可以修改为其他可用端口。

### 🏠 设备标识配置（可选）

// 配套小程序所用到的 设备基本信息
```cpp
#define UserDeviceId "esp8266-bedroom-01"           // 设备唯一标识
#define UserDeviceRoomName "卧室1"                 // 房间名称
#define UserLightDeviceName "卧室大灯"             // 灯设备名称  
#define UserClockDeviceName "客厅时钟"             // 时钟设备名称
```

**建议：** 根据您的实际部署位置修改这些标识，便于在多设备环境中区分。

### 🎛️ 硬件显示配置（可选）

```cpp
#define UserClockBrightnessMin 0                   // 时钟最低亮度
#define UserClockBrightnessMax 15                  // 时钟最高亮度
#define UserClockBrightnessDefault 2               // 时钟默认亮度
#define UserLoveDisplayText "I&Y"                  // 爱心模式显示的文字，& 会在代码中替换为爱心符号
```

## 🛠️ 高级配置

### 系统参数调整
```cpp
#define UserWifiConnectMaxRetry 20                // WiFi连接最大重试次数
#define UserNtpCheckInterval 2000                  // NTP时间校正检查间隔(毫秒)
#define UserWeatherUpdateInterval 3000000          // 天气更新间隔(毫秒，约50分钟)
#define UserUdpBroadcastInterval 5000              // UDP广播间隔(毫秒)
#define UserEepromStartAddress 2500                // EEPROM起始地址
```

</details>

## ⚠️ 注意事项

1. **配置文件安全：** `UserConfig.h` 文件包含敏感信息，已加入 `.gitignore`，不会被提交到版本控制系统。

2. **点灯科技限制：** 每个设备密钥只能同时连接一个设备，请确保密钥的唯一性。

3. **网络要求：** 设备需要稳定的2.4GHz WiFi连接，不支持5GHz网络。

4. **端口冲突：** 如果遇到端口冲突，请修改相应的端口配置。
