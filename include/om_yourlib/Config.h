#pragma once
#include <stdint.h>

namespace om_yourlib {

struct Config {
  // Hardware inputs (do NOT hardcode pins in the library)
  int led_pin = -1;               // optional, used by example behavior
  int uart_rx_pin = -1;           // example: if your lib uses UART
  int uart_tx_pin = -1;

  // Behavior
  uint32_t interval_ms = 1000;    // example periodic tick
};

} // namespace om_yourlib
