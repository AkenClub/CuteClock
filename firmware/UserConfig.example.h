#ifndef _USER_CONFIG_H_
#define _USER_CONFIG_H_

/*
 * CuteClock 用户配置文件
 * 
 * 使用说明：
 * 1. 复制本文件为 UserConfig.h
 * 2. 根据您的实际情况修改以下配置项
 * 3. UserConfig.h 文件不会被提交到git仓库
 */

// =============================================================================
// 🔐 必须配置的敏感信息
// =============================================================================

// WiFi 配置
#define UserWifiSSID "Your_WiFi_Name"          // 修改为您的WiFi名称
#define UserWifiPassword "Your_WiFi_Password"    // 修改为您的WiFi密码

// 点灯科技配置 (需要在点灯科技平台注册设备获取)
#define UserBlinkerAuthKey "your_auth_key"         // 修改为您的点灯科技设备密钥

// =============================================================================
// 🌍 地理位置配置
// =============================================================================

// 天气服务城市代码 (请在心知天气或其他天气API查询您的城市代码)
#define UserWeatherCityCode 440123                 // 修改为您所在城市的代码


// ==================== ⭕ 以下配置项可以不用修改 ⭕ ====================

// =============================================================================
// 🔧 网络服务配置
// =============================================================================

// HTTP 服务器配置
#define UserHttpServerPort 1229                    // HTTP服务器监听端口

// UDP 通信配置
#define UserUdpLocalPort 2506                      // 本地UDP监听端口
#define UserUdpBroadcastPort 1998                  // 远程广播端口

// HTTP API 安全配置
#define UserHttpApiKey "cute_clock_2025"           // HTTP API访问密钥，与配套小程序/Web项目对应，若修改，需要同步修改其他端对应的配置

// =============================================================================
// 🏠 设备标识配置
// =============================================================================

// 配套小程序所用到的 设备基本信息
#define UserDeviceId "esp8266-bedroom-01"           // 设备唯一标识
#define UserDeviceRoomName "卧室1"                 // 房间名称
#define UserLightDeviceName "卧室大灯"             // 灯设备名称  
#define UserClockDeviceName "客厅时钟"             // 时钟设备名称
#define UserPcDeviceName "台式电脑"                // 电脑设备名称

// =============================================================================
// 🎛️ 硬件显示配置
// =============================================================================

// 时钟亮度配置
#define UserClockBrightnessMin 0                   // 时钟最低亮度
#define UserClockBrightnessMax 15                  // 时钟最高亮度
#define UserClockBrightnessDefault 2               // 时钟默认亮度

// 个性化显示
#define UserLoveDisplayText "I&Y"                  // 爱心模式显示的文字，& 在代码中会转为爱心符号

// =============================================================================
// ⚙️ 系统配置
// =============================================================================

// WiFi连接配置
#define UserWifiConnectMaxRetry 20                // WiFi连接最大重试次数

// 定时器配置 (单位：毫秒)
#define UserNtpCheckInterval 2000                  // NTP时间校正检查间隔
#define UserWeatherUpdateInterval 3000000          // 天气更新间隔 (50分钟)
#define UserUdpBroadcastInterval 5000              // UDP广播间隔

// EEPROM配置
#define UserEepromStartAddress 2500                // EEPROM起始地址

#endif // _USER_CONFIG_H_
