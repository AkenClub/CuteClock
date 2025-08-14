// index.js
import Toast from "../../miniprogram_npm/@vant/weapp/toast/toast";
import {
  setUdpListener,
  clearUdpListener,
  getUdpData,
} from "../../other/UdpUtils";
import { CONFIG } from "../../other/config";

// 获取应用实例
const app = getApp();

Page({
  data: {
    serverList: [],
    udpSocket: null,
  },

  onServerClick(e) {
    const server = e.currentTarget.dataset.item;
    app.globalData.remoteTcpServerInfo = server.remoteInfo;
    wx.navigateTo({
      url: "../device-manager/device-manager?id=" + server.id,
    });
  },

  onShow() {
    console.log("index 显示");
    this.data.udpSocket = wx.createUDPSocket();
    const udpSocket = this.data.udpSocket;
    const port = udpSocket.bind(
      CONFIG.udpBroadcastPort || app.globalData.brocastReceive
    );
    setUdpListener(udpSocket, this.udpListener);

    Toast({
      message: `监听广播端口为：${port}`,
      position: "bottom",
    });
  },

  onHide() {
    console.log("index 隐藏");
    clearUdpListener(this.data.udpSocket, this.udpListener);
    this.setData({
      serverList: [],
    });
    this.data.udpSocket.close();
  },

  udpListener(data) {
    const info = getUdpData(data);
    if (!info) return;
    const { udpData, remoteInfo } = info;
    console.log("index got msg:");
    console.log(udpData);
    console.log(remoteInfo);
    console.log("-----------------");

    const device = udpData.data.device;
    const isExist = this.data.serverList.some((item) => {
      return item.remoteInfo.address === remoteInfo.address;
    });
    if (isExist) return;
    const item = {
      type: device.type,
      name: device.data.name,
      remoteInfo: {
        address: remoteInfo.address,
        port: device.data.port,
      },
      id: device.data.id,
      mac: device.data.mac,
      mark: device.data.mark,
    };
    this.data.serverList.push(item);
    this.setData({
      serverList: this.data.serverList,
    });
  },
});
