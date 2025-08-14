let baseUrl = "";
let runtimeApiKey = ""; // 运行时可设置的API密钥（优先级高于配置）
// 专用域名，/ 结尾
const subDomain = "";

module.exports = {
  // 需要 URL 带上 HTTP/HTTPS，有端口号就加上
  setBaseURL: (url) => {
    baseUrl = url;
    console.log(`设置 URL：${baseUrl}`);
  },

  // 运行时设置 API Key（可为空）
  setApiKey: (key) => {
    runtimeApiKey = typeof key === "string" ? key : "";
    console.log(`设置 API Key（长度）: ${runtimeApiKey.length}`);
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
      const effectiveKey = runtimeApiKey; // 不再从配置读取，完全由运行时控制
      const apiKeyHeader = effectiveKey ? { "X-API-Key": effectiveKey } : {};
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
