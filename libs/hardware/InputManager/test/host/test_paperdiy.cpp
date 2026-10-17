#include <BoardConfig.h>
#include <InputManager.h>
#include <PaperDiyBoard.h>
#include <Wire.h>

#include <algorithm>
#include <cassert>
#include <cstdio>

int main() {
  static_assert(BoardConfig::MAX_FRAMEBUFFER_BYTES == 64800);
  static_assert(FREEINK_MCU_S3 && FREEINK_DRIVER_LGFX_EPD && FREEINK_FB_PSRAM);
  static_assert(!FREEINK_CAP_TOUCH && !FREEINK_CAP_RTC && !FREEINK_CAP_AUDIO);
  static_assert(!FREEINK_CAP_FRONTLIGHT && !FREEINK_SD_SDMMC);
  static_assert(BoardConfig::DEFAULT_DEVICE.input.up == 4);
  static_assert(BoardConfig::DEFAULT_DEVICE.input.down == 3);
  static_assert(BoardConfig::DEFAULT_DEVICE.sd.cs == 47);
  std::fill(std::begin(gpioLevels), std::end(gpioLevels), HIGH);
  Wire.fail = true;
  assert(!freeink::paperdiy::ensureBooted());
  Wire.fail = false;
  Wire.registers[0x49] = 0xB6;
  Wire.registers[0x4A] = 0x20;
  assert(freeink::paperdiy::ensureBooted());
  assert(Wire.sda == 41 && Wire.scl == 42);
  assert(Wire.registers[0x49] == 0x37);
  assert(Wire.registers[0x4A] == 0x20);
  assert((Wire.registers[0x10] & 0x0C) == 0x04);
  assert((Wire.registers[0x14] & 0xC0) == 0x40);
  assert(freeink::paperdiy::setEpdPower(true));
  assert(Wire.registers[0x11] & 4);
  assert(freeink::paperdiy::setEpdPower(false));
  assert(!(Wire.registers[0x11] & 4));
  InputManager input;
  input.begin();
  input.update();
  gpioLevels[4] = LOW;
  fakeNow += 30;
  input.update();
  gpioLevels[4] = HIGH;
  fakeNow += 100;
  input.update();
  assert(input.wasPressed(InputManager::BTN_UP));
  assert(input.wasReleased(InputManager::BTN_UP));
  input.update();
  gpioLevels[3] = LOW;
  fakeNow += 30;
  input.update();
  fakeNow += 1500;
  input.update();
  assert(input.isPressed(InputManager::BTN_CONFIRM));
  gpioLevels[3] = HIGH;
  fakeNow += 30;
  input.update();
  assert(input.wasReleased(InputManager::BTN_CONFIRM));
  assert(!input.wasPressed(InputManager::BTN_DOWN));
  fakeNow += 300;
  Wire.registers[0x48] = 0x80;
  input.update();
  assert(input.wasPressed(InputManager::BTN_POWER));
  fakeNow += 30;
  input.update();
  assert(input.wasReleased(InputManager::BTN_POWER));
  fakeNow += 300;
  input.update();
  assert(!input.wasPressed(InputManager::BTN_POWER));
  puts("PaperDIY profile, PMIC recovery, display rail, buttons and one-shot power click passed");
}
