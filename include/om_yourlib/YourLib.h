/**
 * @file YourLib.h
 * @brief Main library class for YourLib.
 *
 * Provides a non-blocking, cooperative library with begin/tick/end lifecycle.
 * All configuration is injected; no hardcoded pins or hidden state.
 */

#pragma once

#include <stdint.h>

#include "om_yourlib/Config.h"
#include "om_yourlib/Status.h"

namespace om_yourlib {

/**
 * @brief Main library class implementing begin/tick/end lifecycle.
 *
 * Usage:
 * @code
 * om_yourlib::YourLib lib;
 *
 * void setup() {
 *   om_yourlib::Config cfg;
 *   cfg.led_pin = 48;
 *   cfg.interval_ms = 1000;
 *   auto st = lib.begin(cfg);
 *   if (!st.ok()) { // handle error }
 * }
 *
 * void loop() {
 *   lib.tick(millis());
 * }
 * @endcode
 *
 * @note This class is not thread-safe. Call all methods from the same
 *       task/thread (typically Arduino loop()).
 * @note Do not call from ISRs.
 */
class YourLib {
 public:
  /**
   * @brief Initialize the library with the given configuration.
   *
   * Must be called before tick(). Can be called again after end() to
   * reinitialize with different settings.
   *
   * @param cfg Configuration struct with pins and parameters.
   * @return Status Ok on success, or error with details.
   *
   * @note Validates cfg.interval_ms > 0.
   * @note If led_pin >= 0, configures GPIO as output.
   */
  Status begin(const Config& cfg);

  /**
   * @brief Stop the library and release resources.
   *
   * Safe to call multiple times. After end(), started() returns false.
   * Call begin() to restart.
   */
  void end();

  /**
   * @brief Cooperative update function. Call from loop().
   *
   * Performs periodic actions based on configured interval. Returns
   * immediately if not enough time has elapsed.
   *
   * @param now_ms Current time in milliseconds (typically from millis()).
   *
   * @note Non-blocking. Safe to call every loop iteration.
   * @note Does nothing if begin() was not called or end() was called.
   */
  void tick(uint32_t now_ms);

  /**
   * @brief Check if library is currently active.
   * @return true if begin() succeeded and end() has not been called.
   */
  bool started() const { return started_; }

 private:
  Config cfg_{};
  bool started_ = false;
  uint32_t next_ms_ = 0;
};

}  // namespace om_yourlib
