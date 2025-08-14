export function getUdpData(data) {
  try {
    const encodedString = String.fromCharCode.apply(
      null,
      new Uint8Array(data.message)
    );
    const msg = decodeURIComponent(escape(encodedString));
    console.log("接收到数据：" + msg, msg.length);
    if (!msg || msg.length <= 0) {
      console.warn("msg 为空！");
      return;
    }
    const server = JSON.parse(msg);
    if (!server) {
      console.warn("非法数据！");
      return;
    }
    if (!server.udp) {
      console.warn("不接受该请求类型");
      return;
    }
    return {
      udpData: server.udp,
      remoteInfo: data.remoteInfo,
    };
  } catch (e) {
    console.warn(e);
  }
}

export function checkUdpData(data) {
  if (data.type !== "SERVER_RESPONSE") {
    throw "非服务器响应！";
  }
  if (data.errCode !== 200) {
    throw data.errmsg || `非正常响应码，${data.errCode}`;
  }
  return data.data;
}

export function setUdpListener(udpSocket, udpListener) {
  console.log("添加 Udp 监听");
  udpSocket.onMessage(udpListener);
}

export function clearUdpListener(udpSocket, udpListener) {
  console.log("移除 Udp 监听");
  udpSocket.offMessage(udpListener);
}
