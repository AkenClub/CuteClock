import { request } from "./base-request";

export function getAllStatus() {
  return request("all-status", {}, "GET");
}

export function setRoomLightStatus(data) {
  return request("room-light", data);
}

export function setClockStatus(data) {
  return request("clock", data);
}

export function triggerPcPower() {
  return request("pc-power", {}, "POST");
}
