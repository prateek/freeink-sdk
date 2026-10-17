#include <LgfxEpdConfig.h>
#include <PaperDiyBoard.h>

namespace {
bool prepareEpdPower() { return freeink::paperdiy::setEpdPower(true); }
}  // namespace

namespace freeink {
const LgfxEpdConfig& paperDiyLgfxConfig() {
  // M5GFX board_M5PaperDIY: stock Bus_EPD sequences OE, boost and SPV.
  static const LgfxEpdConfig config = {
      {6, 14, 7, 12, 9, 11, 8, 10}, 13, 17, 45, 15, 16, 18, 46, 16000000, 8, 0, {&prepareEpdPower, nullptr, nullptr},
  };
  return config;
}
}  // namespace freeink
