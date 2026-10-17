#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <vector>
struct WireStub {
  std::array<uint8_t, 256> registers{};
  std::vector<uint8_t> tx;
  bool fail = false;
  int sda = -1, scl = -1;
  uint8_t reg = 0;
  bool begin(int a, int b, uint32_t) {
    sda = a;
    scl = b;
    return !fail;
  }
  void setTimeOut(int) {}
  void beginTransmission(uint8_t) { tx.clear(); }
  void write(uint8_t b) { tx.push_back(b); }
  int endTransmission(bool = true) {
    if (fail) return 1;
    reg = tx.at(0);
    for (size_t i = 1; i < tx.size(); ++i) registers.at(reg + i - 1) = tx[i];
    return 0;
  }
  int requestFrom(uint8_t, uint8_t count) { return fail ? 0 : count; }
  uint8_t read() {
    auto value = registers.at(reg);
    if (reg == 0x48) registers.at(reg) &= 0x7f;
    ++reg;
    return value;
  }
};
inline WireStub Wire;
