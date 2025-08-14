export function getClockBrightnessParams(id, level) {
  if (typeof id === 'string' && typeof level === 'number') {
    return {
      id: id,
      order: {
        type: 'brightness',
        value: level,
      },
    }
  } else {
    console.log('获取 ClockBrightnessParams 参数失败，检查入参！')
    return null
  }
}
