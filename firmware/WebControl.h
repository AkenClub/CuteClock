#ifndef _WEB_CONTROL_H_
#define _WEB_CONTROL_H_

// 极简控制页面 HTML
const char WEB_CONTROL_HTML[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>CuteClock 控制</title>
<style>
body{font-family:Arial,sans-serif;margin:0;padding:20px;background:#f0f2f5;color:#333}
.container{max-width:480px;margin:0 auto}
h1{text-align:center;color:#667eea;margin-bottom:20px}
.card{background:#fff;border-radius:8px;padding:15px;margin:10px 0;box-shadow:0 2px 8px rgba(0,0,0,0.1)}
.control{display:flex;justify-content:space-between;align-items:center;margin:10px 0}
.btn{background:#667eea;color:#fff;border:none;padding:8px 16px;border-radius:5px;cursor:pointer;font-size:14px}
.btn:hover{background:#5a6fd8}
.btn.off{background:#ccc}
.temp{font-size:24px;color:#667eea;text-align:center;margin:10px 0}
.range{width:100%;margin:10px 0}
.status{font-size:12px;color:#666;margin-top:5px}
.settings{background:#f8f9fa;border:1px solid #dee2e6}
.settings input{width:calc(100% - 70px);padding:8px;border:1px solid #ddd;border-radius:3px}
.settings button{width:64px;margin-left:8px}
</style>
</head>
<body>
<div class="container">
<h1>🕐 CuteClock</h1>

<div class="card settings">
<div>⚙️ 设置</div>
<div class="control">
<input type="password" id="api-key" placeholder="请输入API密钥" onkeyup="saveApiKey()">
<button class="btn" onclick="clearApiKey()">清除</button>
</div>
<div class="status">控制设备需要API密钥，输入后自动保存</div>
</div>

<div class="card">
<div>💡 灯光控制</div>
<div class="control">
<span id="light-name">房间灯</span>
<button id="light-btn" class="btn" onclick="toggleLight()">--</button>
</div>
<div class="status" id="light-status">正在加载...</div>
</div>

<div class="card">
<div>🕐 时钟亮度</div>
<div class="control">
<span id="clock-name">时钟</span>
<span id="brightness-value">0</span>
</div>
<input type="range" id="brightness-range" class="range" min="0" max="15" value="0" oninput="setBrightness(this.value)">
</div>

<div class="card">
<div>🖥️ 电脑电源</div>
<div class="control">
<span id="pc-name">电脑</span>
<button class="btn" onclick="triggerPC()">触发开关</button>
</div>
</div>

<div class="card">
<div>🌡️ 室内温度</div>
<div class="temp" id="temperature">--°C</div>
</div>

</div>

<script>
let API_KEY=localStorage.getItem('cuteclock_api_key')||'';
let BASE_URL=window.location.origin;
let devices={};

async function req(url,data){
const opt={method:data?'POST':'GET',headers:{'Content-Type':'application/json'}};
if(data&&API_KEY){opt.body=JSON.stringify(data);opt.headers['X-API-Key']=API_KEY;}
else if(data){opt.body=JSON.stringify(data);}
const res=await fetch(BASE_URL+url,opt);
if(!res.ok){
throw new Error(`HTTP ${res.status}`);
}
return res.json();
}

function saveApiKey(){
const key=document.getElementById('api-key').value.trim();
API_KEY=key;
if(key){
localStorage.setItem('cuteclock_api_key',key);
}else{
localStorage.removeItem('cuteclock_api_key');
}
}

function clearApiKey(){
document.getElementById('api-key').value='';
API_KEY='';
localStorage.removeItem('cuteclock_api_key');
}

async function loadStatus(){
try{
const data=await req('/all-status');
devices={};
data.data.items.forEach(item=>{devices[item.type]=item;});
updateUI();
}catch(e){
console.error(e);
}
}

function updateUI(){
if(devices.light){
document.getElementById('light-name').textContent=devices.light.mark;
const btn=document.getElementById('light-btn');
const isOn=devices.light.data.enable;
btn.textContent=isOn?'关闭':'开启';
btn.className=isOn?'btn':'btn off';
document.getElementById('light-status').textContent=isOn?'已开启':'已关闭';
}
if(devices.clock){
document.getElementById('clock-name').textContent=devices.clock.mark;
document.getElementById('brightness-value').textContent=devices.clock.data.brightness;
document.getElementById('brightness-range').value=devices.clock.data.brightness;
}
if(devices.pc){
document.getElementById('pc-name').textContent=devices.pc.mark;
}
if(devices.temperature){
document.getElementById('temperature').textContent=devices.temperature.data.value+devices.temperature.data.unit;
}
}

async function toggleLight(){
if(!devices.light)return;
if(!API_KEY){
alert('请先在设置中输入API密钥');
return;
}
const enable=!devices.light.data.enable;
try{
await req('/room-light',{id:devices.light.id,order:{type:'enable',value:enable?'on':'off'}});
setTimeout(loadStatus,200);
}catch(e){alert('操作失败: '+e.message);}
}

async function setBrightness(val){
document.getElementById('brightness-value').textContent=val;
if(!devices.clock)return;
if(!API_KEY){
alert('请先在设置中输入API密钥');
return;
}
try{
await req('/clock',{id:devices.clock.id,order:{type:'brightness',value:parseInt(val)}});
}catch(e){alert('设置失败: '+e.message);}
}

async function triggerPC(){
if(!API_KEY){
alert('请先在设置中输入API密钥');
return;
}
try{
await req('/pc-power');
alert('已触发电脑电源开关');
}catch(e){alert('操作失败: '+e.message);}
}

// 页面加载时恢复API密钥设置
if(localStorage.getItem('cuteclock_api_key')){
document.getElementById('api-key').value=localStorage.getItem('cuteclock_api_key');
}

loadStatus();
setInterval(loadStatus,5000);
</script>
</body>
</html>
)";

#endif // _WEB_CONTROL_H_
