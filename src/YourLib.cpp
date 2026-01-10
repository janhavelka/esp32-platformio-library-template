/**
 * @file YourLib.cpp
 * @brief Implementation of YourLib.
 */

#include "YourLibrary/YourLib.h"

#include <Arduino.h>

namespace YourLibrary {

Status YourLib::begin(const Config& cfg) {
  // Validate configuration
  if (cfg.interval_ms == 0) {
    return {Err::INVALID_CONFIG, 0, "interval_ms must be > 0"};
  }

  cfg_ = cfg;
  started_ = true;
  next_ms_ = 0;

  // Configure LED pin if provided
  if (cfg_.led_pin >= 0) {
    pinMode(static_cast<uint8_t>(cfg_.led_pin), OUTPUT);
    digitalWrite(static_cast<uint8_t>(cfg_.led_pin), LOW);
  }

  return Ok();
}

void YourLib::end() {
  if (!started_) {
    return;
  }

  started_ = false;

  // Turn off LED if configured
  if (cfg_.led_pin >= 0) {
    digitalWrite(static_cast<uint8_t>(cfg_.led_pin), LOW);
  }
}

void YourLib::tick(uint32_t now_ms) {
  if (!started_) {
    return;
  }

  if (now_ms < next_ms_) {
    return;
  }

  next_ms_ = now_ms + cfg_.interval_ms;

  // Example action: toggle LED if configured
  if (cfg_.led_pin >= 0) {
    const uint8_t pin = static_cast<uint8_t>(cfg_.led_pin);
    const int current = digitalRead(pin);
    digitalWrite(pin, current ? LOW : HIGH);
  }
}

}  // namespace YourLibrary
