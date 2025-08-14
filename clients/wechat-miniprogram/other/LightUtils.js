export function getLightEnableParams(id, isEnable) {
  if (typeof id === 'string' && typeof isEnable === 'boolean') {
    return {
      id: id,
      order: {
        type: 'enable',
        value: isEnable,
      },
    }
  } else {
    console.log('获取 LightEnable 参数失败，检查入参！')
    return null
  }
}
