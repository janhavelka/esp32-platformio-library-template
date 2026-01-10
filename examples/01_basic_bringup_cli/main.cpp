#include <Arduino.h>
#include "examples/common/Log.h"
#include "examples/common/BoardPins.h"
#include "om_yourlib/YourLib.h"

static om_yourlib::YourLib g_lib;

static String read_line() {
  static String s;
  while (Serial.available()) {
    char c = (char)Serial.read();
    if (c == '\r') continue;
    if (c == '\n') { String out = s; s = ""; return out; }
    s += c;
  }
  return "";
}

static void help() {
  Serial.println("Commands:");
  Serial.println("  help");
  Serial.println("  start <ms>");
  Serial.println("  stop");
}

void setup() {
  log_begin(115200);
  delay(50);
  help();
}

void loop() {
  const uint32_t now = millis();
  g_lib.tick(now);

  const String line = read_line();
  if (line.length() == 0) return;

  if (line == "help") { help(); return; }

  if (line.startsWith("start ")) {
    const uint32_t ms = (uint32_t)line.substring(6).toInt();
    om_yourlib::Config cfg;
    cfg.interval_ms = (ms == 0) ? 1000 : ms;
    cfg.led_pin = pins::LED;

    const auto st = g_lib.begin(cfg);
    if (!st.ok()) LOGE("begin failed: %s", st.msg);
    else LOGI("started interval=%lu", (unsigned long)cfg.interval_ms);
    return;
  }

  if (line == "stop") {
    g_lib.end();
    LOGI("stopped");
    return;
  }

  LOGE("unknown; type help");
}
