#pragma once

#include <M5Pm1.h>

namespace freeink::paperdiy {

inline bool ensureBooted() {
  static bool ready = false;
  if (ready) return true;
  using namespace m5pm1;
  beginBus();
  // PYG2 supplies the EPD; PYG3 is the active-low charger status input.
  ready = updateRegChecked(REG_GPIO_FUNC0, 0xF0, 0) && updateRegChecked(REG_GPIO_MODE, 0x08, 0x04) &&
          updateRegChecked(REG_GPIO_DRV, 0x04, 0) && updateRegChecked(REG_GPIO_PULL0, 0xC0, 0x40) &&
          configureAppPowerButton();
  return ready;
}

inline bool setEpdPower(bool enabled) {
  if (!ensureBooted()) return false;
  if (!m5pm1::updateRegChecked(m5pm1::REG_GPIO_OUT, 0x04, enabled ? 0x04 : 0)) return false;
  delay(10);
  return true;
}

inline bool pollPowerButtonClicked(unsigned long nowMs) {
  static unsigned long lastPollMs = 0;
  if (nowMs - lastPollMs < 250) return false;
  lastPollMs = nowMs;
  if (!ensureBooted()) return false;
  uint8_t status = 0;
  return m5pm1::readButtonStatus(&status) && (status & m5pm1::BTN_EVENT) != 0;
}

}  // namespace freeink::paperdiy
