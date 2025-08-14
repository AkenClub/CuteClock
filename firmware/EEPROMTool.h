#ifndef EEPROM_TOOL_H
#define EEPROM_TOOL_H

#include <EEPROM.h>
#include "UserConfig.h"

#define EEPROM_BEGIN UserEepromStartAddress

struct EEPROMTOOLS
{

  /**
   * 保存数据
   */
  void saveData(uint8_t *data, int offset, int length)
  {
    EEPROM.begin(EEPROM_BEGIN);
    for (int i = 0; i < length; i++)
    {
      EEPROM.write(offset + i, data[i] & 0xff);
    }
    EEPROM.commit();
  }

  /**
   *获取数据
   */
  uint8_t *loadData(int offset, int length)
  {
    unsigned char *arr = new uint8_t[length];
    EEPROM.begin(EEPROM_BEGIN);
    for (int i = 0; i < length; i++)
    {
      arr[i] = EEPROM.read(offset + i);
    }
    return arr;
  }

  /**
   * 删除数据
   */
  void clearData(int offset, int length)
  {
    EEPROM.begin(EEPROM_BEGIN);
    for (int i = 0; i < length; i++)
    {
      EEPROM.write(offset + i, 0x0);
    }
    EEPROM.commit();
  }

  /**
   * 删除全部数据
   */
  void clearAll()
  {
    EEPROM.begin(EEPROM_BEGIN);
    for (int i = 0; i < EEPROM_BEGIN; i++)
    {
      EEPROM.write(i, 0x0);
    }
    EEPROM.commit();
  }
};

static EEPROMTOOLS EEPROMTool;

#endif