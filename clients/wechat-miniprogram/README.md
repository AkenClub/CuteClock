# 📱 微信小程序客户端

CuteClock 微信小程序版本，提供便捷的移动端设备控制。

## ✨ 功能特性

- 🏠 设备控制：房间灯光开关
- 🔆 亮度调节：时钟显示亮度
- 🖥️ 电脑控制：远程电脑电源（切换动作）
- 📡 局域网发现：通过 UDP 自动发现设备
- 🌡️ 温度显示: 室内温度监测

## ⚙️ 配置说明（重要）

小程序已将端口配置项集中到 `other/config.js`，可以全部保持默认：

```js
export const CONFIG = {
  defaultPort: 1229, // 固件 HTTP 端口（与固件一致）
  udpBroadcastPort: 1998, // UDP 广播接收端口（与固件一致）
  defaultDeviceAddress: "" // 可选：固定设备 IP（留空表示使用自动发现）
};
```

保存以上配置后重新编译预览即可生效。

## 🧹 隐私与安全

- 已移除 `project.config.json` 中的 `appid`，请在本地开发工具中自行填写
- 请勿提交个人的 `project.private.config.json`
- 如设置了 `apiKey`，请妥善保管，不要泄漏

## 🛠️ 技术栈

- 微信小程序原生开发
- Vant Weapp 组件库
- wx.request 封装 + JSON API

## 🚀 运行与调试

1. 使用微信开发者工具导入本目录
2. 在开发者工具中填写 `appid`
3. 在 CuteClock\clients\wechat-miniprogram 目录下，终端中执行：`npm install`
4. 在微信开发者工具中：工具 → 构建 npm
5. 构建完成后重新编译即可见到效果
6. 如需固定设备：在 `other/config.js` 中设置 `defaultDeviceAddress`
7. 如需鉴权：在 设备管理页面 中设置 `API Key`，保存后会自动保存到本地

**自用提示：** 如果只是自用，可以预览使用手机扫码后，用开发版本即可

## 🔗 相关链接

- 📖 [HTTP API 文档](../../docs/api-usage.md)
- 🏠 [项目主页](../../README.md)
- 📱 [微信小程序开发文档](https://developers.weixin.qq.com/miniprogram/dev/framework/)
- 🧩 [Vant Weapp 组件库文档](https://vant-ui.github.io/vant-weapp/#/quickstart)
