#pragma once
#include <Arduino.h>
#include "examples/common/BuildConfig.h"

inline void log_begin(unsigned long baud = 115200) {
  Serial.begin(baud);
}

#define LOGE(fmt, ...) do { if (LOG_LEVEL >= 1) Serial.printf("[E] " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOGI(fmt, ...) do { if (LOG_LEVEL >= 2) Serial.printf("[I] " fmt "\n", ##__VA_ARGS__); } while(0)
#define LOGD(fmt, ...) do { if (LOG_LEVEL >= 3) Serial.printf("[D] " fmt "\n", ##__VA_ARGS__); } while(0)
