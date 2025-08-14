import Toast from "../../miniprogram_npm/@vant/weapp/toast/toast";
import { getLightEnableParams } from "../../other/LightUtils";
import { getClockBrightnessParams } from "../../other/ClockUtils";
import {
  getAllStatus,
  setRoomLightStatus,
  setClockStatus,
  triggerPcPower,
} from "../../http/wifi-server-api";
import Dialog from "../../miniprogram_npm/@vant/weapp/dialog/dialog";
import { setBaseURL, setApiKey } from "../../http/base-request";
import { CONFIG } from "../../other/config";

const app = getApp();

Page({
  /**
   * 页面的初始数据
   */
  data: {
    toast: null,
    tcp: null,
    brightness: 1,
    pingTimeout: 0,
    pingTask: null,
    devicesList: {
      light: [],
      clock: [],
      temperature: [],
      pc: [],
    },
    isPcTriggering: false,
    lastPcTriggerAt: 0,
    apiKeyInput: "",
    currentDeviceId: "",
  },

  onLoad(options) {
    const id = options.id;
    console.log("device manager 加载", id);
    const deviceId = id || "__default__";
    this.setData({ currentDeviceId: deviceId });
    try {
      const savedKey = wx.getStorageSync(deviceId) || "";
      this.setData({ apiKeyInput: savedKey });
      setApiKey(savedKey);
    } catch (e) {
      console.warn("读取API Key失败", e);
    }
  },

  /**
   * 生命周期函数--监听页面显示
   */
  onShow: function () {
    console.log("device manager 显示");
    this.showLoading();

    wx.setKeepScreenOn({
      keepScreenOn: true,
      fail() {
        console.log("开启常亮失败");
      },
    });

    const { address, port } = app.globalData.remoteTcpServerInfo;
    const baseAddress = address || CONFIG.defaultDeviceAddress;
    const basePort = port || CONFIG.defaultPort;
    setBaseURL(`http://${baseAddress}:${basePort}`);

    this.refreshAllStatus();
  },

  onApiKeyInput(e) {
    this.setData({ apiKeyInput: (e.detail && e.detail.value) || "" });
  },

  onSaveApiKey() {
    const key = this.data.apiKeyInput || "";
    const deviceId = this.data.currentDeviceId || "__default__";
    try {
      wx.setStorageSync(deviceId, key);
      setApiKey(key);
      this.showSuccessLoading();
    } catch (e) {
      console.error("保存API Key失败", e);
      this.showFailLoading("保存失败");
    }
  },

  refreshAllStatus() {
    this.showLoading();

    getAllStatus()
      .then((res) => {
        this.handleResponseAllStatus(res.data.items);
        this.hideLoading();
      })
      .catch((e) => {
        console.error(`获取所有状态失败:${e}`);
        this.showFailLoading(`接口异常，${e}`);
      })
      .finally(() => {
        wx.stopPullDownRefresh();
      });
  },

  onLightChange(e) {
    const lightItem = e.currentTarget.dataset.lightitem;
    const data = getLightEnableParams(lightItem.id, e.detail);

    this.showLoading();
    setRoomLightStatus(data)
      .then((res) => {
        console.log("设置灯状态成功");
        this.updateLightStatus(lightItem.id, e.detail);
        this.showSuccessLoading();
      })
      .catch((e) => {
        console.error("设置灯状态失败：" + e);
        this.showFailLoading(`${e}`);
      });
  },

  updateLightStatus(id, isEnable) {
    const foundIndex = this.data.devicesList.light.findIndex((item) => {
      return item.id === id;
    });
    if (foundIndex === -1) {
      return;
    }
    this.data.devicesList.light[foundIndex].enable = isEnable;
    this.setData({
      devicesList: this.data.devicesList,
    });
  },

  onSliderDrag(event) {
    const clockItem = event.currentTarget.dataset.clockitem;
    this.updateClockStatus(clockItem.id, event.detail.value);
  },

  onSliderChange(event) {
    const clockItem = event.currentTarget.dataset.clockitem;
    const id = clockItem.id;
    const value = event.detail;
    const data = getClockBrightnessParams(id, value);

    this.showLoading();
    setClockStatus(data)
      .then(() => {
        console.log("设置成功");
        this.updateClockStatus(id, value, true);
        this.showSuccessLoading();
      })
      .catch((e) => {
        console.error(`设置失败，${e}`);
        // 设置值恢复为之前的
        console.warn(clockItem);
        this.updateClockStatus(clockItem.id, clockItem.brightness);
        this.showFailLoading(e);
      });
  },

  onTriggerPcPower() {
    if (this.data.isPcTriggering) {
      return;
    }
    const now = Date.now();
    if (now - this.data.lastPcTriggerAt < 3000) {
      this.showFailLoading("操作太频繁，请稍后再试");
      return;
    }

    Dialog.confirm({
      title: "确认操作",
      message: "将触发电脑电源开关（可能开机/关机），是否继续？",
      confirmButtonText: "确定",
      cancelButtonText: "取消",
    })
      .then(() => {
        this.setData({ lastPcTriggerAt: Date.now() });
        this.setData({ isPcTriggering: true });
        this.showLoading();
        return triggerPcPower();
      })
      .then(() => {
        this.showSuccessLoading();
      })
      .catch((e) => {
        console.log(e);
        if (e.closeAction !== "cancel") {
          this.showFailLoading(`${e}`);
        }
      })
      .finally(() => {
        this.setData({ isPcTriggering: false });
      });
  },

  updateClockStatus(id, value, isUpdateReal) {
    const foundIndex = this.data.devicesList.clock.findIndex((item) => {
      return item.id === id;
    });
    if (foundIndex === -1) {
      return;
    }
    this.data.devicesList.clock[foundIndex].brightnessTmp = value;
    if (isUpdateReal) {
      this.data.devicesList.clock[foundIndex].brightness = value;
    }
    this.setData({
      devicesList: this.data.devicesList,
    });
  },

  /**
   * 页面相关事件处理函数--监听用户下拉动作
   */
  onPullDownRefresh: function () {
    console.log("refresh...");
    this.refreshAllStatus();
  },

  handleResponseAllStatus(statusList) {
    console.log(`处理 获取所有状态 的响应体`);
    console.log(statusList);
    this.data.devicesList.light.length = 0;
    this.data.devicesList.clock.length = 0;
    this.data.devicesList.temperature.length = 0;
    this.data.devicesList.pc.length = 0;
    statusList.forEach((item) => {
      switch (item.type) {
        case "light":
          this.data.devicesList.light.push({
            id: item.id,
            mark: item.mark,
            enable: item.data.enable,
          });
          break;
        case "clock":
          this.data.devicesList.clock.push({
            id: item.id,
            mark: item.mark,
            brightness: item.data.brightness,
            brightnessTmp: item.data.brightness,
          });
          break;
        case "temperature":
          this.data.devicesList.temperature.push({
            id: item.id,
            mark: item.mark,
            value: item.data.value,
            unit: item.data.unit,
          });
          break;
        case "pc":
          this.data.devicesList.pc.push({
            id: item.id,
            mark: item.mark,
            enable: item.data.enable,
            description: item.data.description,
          });
          break;
        default:
          break;
      }
    });
    this.setData({
      devicesList: this.data.devicesList,
    });
  },

  showLoading() {
    Toast.loading({
      message: "加载中...",
      forbidClick: true,
      duration: 0,
    });
  },

  showSuccessLoading() {
    Toast.success({
      message: "设置成功",
      duration: 800,
    });
  },

  showFailLoading(errMsg) {
    Toast.fail(errMsg);
  },

  hideLoading() {
    Toast.clear();
  },
});
