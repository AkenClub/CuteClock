# 🔌 HTTP API 使用指南

本文档详细说明如何使用 CuteClock 的 HTTP API 接口控制设备。

## 🔒 安全特性

### API 密钥保护

（固件设置的密钥为空字符串时，不进行密钥验证）

- **可选的接口保护**：房间灯光、时钟亮度、电脑电源等敏感接口支持 API 密钥验证。
- **跳过验证**：在固件的 `UserConfig.h` 中将 `UserHttpApiKey` 设置为空字符串 (`""`)，可以跳过所有接口的密钥验证，方便在受信任的内网环境中进行调试或使用。
- **请求头验证**：启用密钥时，使用 `X-API-Key` 请求头传递密钥。

### 安全建议

- ✅ **内网部署**：无论是否启用密钥，都强烈建议在内网环境中使用，避免将设备暴露到公网。
- ✅ **访问控制**：通过路由器等设备限制只有可信的设备可以访问。

### 接口权限说明

| 接口          | 访问权限          | 用途              |
| ------------- | ----------------- | ----------------- |
| `/`           | 🟢 公开           | 内置 Web 控制面板 |
| `/all-status` | 🟢 公开           | 查看设备状态      |
| `/room-light` | 🔐 可配置密钥验证 | 控制房间灯光      |
| `/clock`      | 🔐 可配置密钥验证 | 调节时钟亮度      |
| `/pc-power`   | 🔐 可配置密钥验证 | 控制电脑电源      |

## 🔐 API 安全认证

### 密钥配置

在开始使用 API 之前，请确保已在 `UserConfig.h` 文件中配置了您的 API 密钥：

```cpp
#define UserHttpApiKey "your_unique_api_key_here"
```

**重要提示：**

- **设置唯一密钥**：为了安全，建议设置一个唯一的 API 密钥，不要使用默认值。
- **禁用认证**：如果您在完全受信任的内部网络中使用，可以将此密钥设置为空字符串 (`""`) 来禁用认证，从而简化 API 调用。
- **密钥保密**：不要将密钥泄露给他人。
- **Web UI**：内置 Web 控制面板需要用户手动输入密钥（如果已设置）。

### 使用方法

对于需要认证的接口（即 `UserHttpApiKey` 非空时），在 HTTP 请求头中加入：

```
X-API-Key: your_api_key_here
```

如果认证被禁用，则无需添加此请求头。

## 📡 API 接口详情

### 基础信息

- **服务器地址**: `http://设备IP地址:端口号`
- **默认端口**: `1229`（可在配置文件中修改）
- **数据格式**: JSON
- **字符编码**: UTF-8

## 🎛️ 接口列表

### 1. 设备状态检测

**简单的在线状态检测接口**

- **URL**: `/`
- **方法**: `GET`
- **认证**: 无需密钥
- **参数**: 无

**示例请求:**

```bash
curl http://192.168.1.100:1229/
```

**响应:**

```
返回内置 Web 控制面板
```

---

### 2. 获取设备状态

**获取设备完整状态信息，包括房间灯光、时钟显示、温度传感器等设备状态**

- **URL**: `/all-status`
- **方法**: `GET`
- **认证**: 无需密钥
- **参数**: 无

**设备类型说明:**

| 类型          | 描述           | 数据字段                                                                             |
| ------------- | -------------- | ------------------------------------------------------------------------------------ |
| `light`       | 房间灯光状态   | `enable`: 是否开启 (boolean)                                                         |
| `clock`       | 时钟显示状态   | `enable`: 是否启用 (boolean)<br>`brightness`: 亮度等级 <br>`mode`: 显示模式 (number) |
| `pc`          | 电脑设备状态   | `enable`: 是否可用 (boolean)<br>`description`: 功能描述 (string)                     |
| `temperature` | 温度传感器数据 | `value`: 温度值 (number)<br>`unit`: 温度单位 (string, "°C")                          |

**示例请求:**

```bash
curl http://192.168.1.100:1229/all-status
```

**响应示例:**

```json
{
  "errCode": 0,
  "errMsg": "获取成功",
  "data": {
    "id": "esp8266-bedroom-01",
    "order": "RESPONSE_ALL_STATUS",
    "items": [
      {
        "type": "light",
        "id": "light-bedroom-01",
        "mark": "卧室大灯",
        "data": {
          "enable": true
        }
      },
      {
        "type": "clock",
        "id": "clock-living_room-01",
        "mark": "客厅时钟",
        "data": {
          "enable": true,
          "brightness": 5,
          "mode": 1
        }
      },
      {
        "type": "pc",
        "id": "pc-desktop-01",
        "mark": "台式电脑",
        "data": {
          "enable": true,
          "description": "电源开关控制"
        }
      },
      {
        "type": "temperature",
        "id": "temperature-sensor-01",
        "mark": "室内温度",
        "data": {
          "value": 24.5,
          "unit": "°C"
        }
      }
    ]
  }
}
```

---

### 3. 控制房间灯光

**控制房间灯具的开关状态**

- **URL**: `/room-light`
- **方法**: `POST`
- **认证**: 🔐 **可配置密钥验证**
- **Content-Type**: `application/json`

**请求参数:**

```json
{
  "id": "light-bedroom-01",
  "order": {
    "type": "enable",
    "value": "on" // "on" 或 "off"
  }
}
```

**示例请求:**

```bash
# 开灯
curl -X POST http://192.168.1.100:1229/room-light \
  -H "X-API-Key: your_api_key_here" \
  -H "Content-Type: application/json" \
  -d '{
    "id": "light-bedroom-01",
    "order": {
      "type": "enable",
      "value": "on"
    }
  }'

# 关灯
curl -X POST http://192.168.1.100:1229/room-light \
  -H "X-API-Key: your_api_key_here" \
  -H "Content-Type: application/json" \
  -d '{
    "id": "light-bedroom-01",
    "order": {
      "type": "enable",
      "value": "off"
    }
  }'
```

**成功响应:**

```json
{
  "errCode": 0,
  "errMsg": "成功"
}
```

---

### 4. 调节时钟亮度

**调节 LED 显示屏的亮度级别**

- **URL**: `/clock`
- **方法**: `POST`
- **认证**: 🔐 **可配置密钥验证**
- **Content-Type**: `application/json`

**请求参数:**

```json
{
  "id": "clock-living_room-01",
  "order": {
    "type": "brightness",
    "value": 5
  }
}
```

**示例请求:**

```bash
# 设置为中等亮度
curl -X POST http://192.168.1.100:1229/clock \
  -H "X-API-Key: your_api_key_here" \
  -H "Content-Type: application/json" \
  -d '{
    "id": "clock-living_room-01",
    "order": {
      "type": "brightness",
      "value": 8
    }
  }'
```

**成功响应:**

```json
{
  "errCode": 0,
  "errMsg": "成功"
}
```

---

### 5. 控制电脑电源

**模拟按下电脑电源开关按钮**

- **URL**: `/pc-power`
- **方法**: `POST`
- **认证**: 🔐 **可配置密钥验证**
- **Content-Type**: `application/json`

**功能说明:**

- **无状态检测**：无法判断电脑当前是开机还是关机状态
- **工作原理**：模拟人手按下电脑电源开关按钮
- **实际效果**：
  - 如果电脑处于关机状态，调用接口会开机
  - 如果电脑处于开机状态，调用接口会触发关机/休眠
  - 相当于物理按下电源开关的效果

**示例请求:**

```bash
# 触发电脑电源开关
curl -X POST http://192.168.1.100:1229/pc-power \
  -H "X-API-Key: your_api_key_here" \
  -H "Content-Type: application/json"
```

**成功响应:**

```json
{
  "errCode": 0,
  "errMsg": "成功"
}
```

**⚠️ 重要提示：**

- 此接口不需要请求参数
- 每次调用都会发送 500 毫秒的电源脉冲信号
- 建议避免频繁调用，给电脑足够的响应时间

## ❌ 错误处理

### 常见错误码

| 错误码 | 说明       | 解决方法              |
| ------ | ---------- | --------------------- |
| `-1`   | 通用错误   | 检查请求参数和格式    |
| `401`  | 认证失败   | 检查 API 密钥是否正确 |
| `404`  | 接口不存在 | 检查请求路径是否正确  |

### 错误响应格式

```json
{
  "errCode": -1,
  "errMsg": "错误描述信息"
}
```

### 常见错误示例

**缺少 API 密钥:**

```json
{
  "errCode": -1,
  "errMsg": "缺少API密钥"
}
```

**API 密钥无效:**

```json
{
  "errCode": -1,
  "errMsg": "API密钥无效"
}
```

**参数格式错误:**

```json
{
  "errCode": -1,
  "errMsg": "时钟状态解析失败或设置值非法"
}
```

## 🧪 测试工具推荐

### 1. curl (命令行)

适合脚本自动化和快速测试

```bash
curl -X POST http://设备IP:1229/room-light \
  -H "X-API-Key: your_key" \
  -H "Content-Type: application/json" \
  -d '{"id":"light-bedroom-01","order":{"type":"enable","value":"on"}}'
```

### 2. Postman (图形界面)

1. 新建 POST 请求
2. URL: `http://设备IP:1229/room-light`
3. Headers 添加:
   - `X-API-Key`: `your_api_key`
   - `Content-Type`: `application/json`
4. Body 选择 raw/JSON，输入请求参数

### 3. JavaScript (网页/Node.js)

```javascript
const response = await fetch("http://设备IP:1229/room-light", {
  method: "POST",
  headers: {
    "X-API-Key": "your_api_key",
    "Content-Type": "application/json",
  },
  body: JSON.stringify({
    id: "light-bedroom-01",
    order: {
      type: "enable",
      value: "on",
    },
  }),
});

const result = await response.json();
console.log(result);
```

### 4. Python

```python
import requests

# 控制房间灯光
url = 'http://设备IP:1229/room-light'
headers = {
    'X-API-Key': 'your_api_key',
    'Content-Type': 'application/json'
}
data = {
    'id': 'light-bedroom-01',
    'order': {
        'type': 'enable',
        'value': 'on'
    }
}

response = requests.post(url, headers=headers, json=data)
print(response.json())

# 控制电脑电源
pc_url = 'http://设备IP:1229/pc-power'
pc_response = requests.post(pc_url, headers=headers)
print(pc_response.json())
```

## 🔧 故障排除

### 连接问题

1. **确认设备在线**: 访问 `/` 接口检查设备状态
2. **检查网络**: 确保设备和客户端在同一网络
3. **端口检查**: 确认端口号配置正确（默认 1229）

### 认证问题

1. **检查密钥**: 确认配置文件中的密钥与请求头中的密钥一致。
2. **确认是否禁用**: 检查固件中的 `UserHttpApiKey` 是否被设置为空字符串 `""`，如果是，则所有接口都不需要密钥。
3. **请求头格式**: 确保使用 `X-API-Key` 而不是其他名称。
4. **密钥字符**: 检查是否有特殊字符或空格。

### 参数问题

1. **JSON 格式**: 使用 JSON 格式验证工具检查参数格式
2. **参数值**: 确认参数值在有效范围内
3. **Content-Type**: 确保设置了正确的 Content-Type 头

---

**更多帮助请参考：**

- 📖 [配置文件说明](configuration.md)
- 🏠 [硬件搭建指南](hardware-setup.md)
- 💻 [软件配置指南](software-setup.md)
