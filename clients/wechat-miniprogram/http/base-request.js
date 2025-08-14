const { CONFIG } = require("../other/config");

let baseUrl = "";
// 专用域名，/ 结尾
const subDomain = "";

module.exports = {
  // 需要 URL 带上 HTTP/HTTPS，有端口号就加上
  setBaseURL: (url) => {
    baseUrl = url;
    console.log(`设置 URL：${baseUrl}`);
  },

  // 二次封装wx.request
  // {String }url:请求的接口地址，开头不用 /
  // {String} method:请求方式 GET,POST....
  // {Object} data:要传递的参数
  // { boolean }isSubDomain:表示是否添加二级子域名 true代表添加,false代表不添加

  //二次封装wx.request
  request: (url, data = {}, method = "POST", isSubDomain = false) => {
    return new Promise((resolve, reject) => {
      // 拼接
      const apiKeyHeader = CONFIG.apiKey ? { "X-API-Key": CONFIG.apiKey } : {};
      let _url = `${baseUrl}/${isSubDomain ? subDomain : ""}${url}`;
      wx.request({
        url: _url,
        data: data,
        method: method,
        header: {
          "content-type": "application/json",
          ...apiKeyHeader,
        },
        success: (res) => {
          console.log("获取数据", res);
          let { errCode, errMsg } = res.data;
          if (errCode != 0) {
            reject(errMsg);
            return;
          }
          resolve(res.data);
        },
        //失败回调
        fail: () => {
          //失败抛出
          reject("请求失败");
        },
      });
    });
  },
};
