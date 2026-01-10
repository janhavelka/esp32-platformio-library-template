#include "om_yourlib/YourLib.h"
#include <Arduino.h>

namespace om_yourlib {

Status YourLib::begin(const Config& cfg) {
  if (cfg.interval_ms == 0) {
    return {Err::InvalidArg, 0, "interval_ms must be > 0"};
  }

  cfg_ = cfg;
  started_ = true;
  next_ms_ = 0;

  // Optional example behavior: LED pin if provided
  if (cfg_.led_pin >= 0) {
    pinMode(cfg_.led_pin, OUTPUT);
    digitalWrite(cfg_.led_pin, LOW);
  }

  return Ok();
}

void YourLib::end() {
  started_ = false;
  if (cfg_.led_pin >= 0) {
    digitalWrite(cfg_.led_pin, LOW);
  }
}

void YourLib::tick(uint32_t now_ms) {
  if (!started_) return;
  if (now_ms < next_ms_) return;

  next_ms_ = now_ms + cfg_.interval_ms;

  // Example action: toggle LED if configured
  if (cfg_.led_pin >= 0) {
    const int v = digitalRead(cfg_.led_pin);
    digitalWrite(cfg_.led_pin, v ? LOW : HIGH);
  }
}

} // namespace om_yourlib
