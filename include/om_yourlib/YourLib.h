#pragma once
#include <stdint.h>
#include "om_yourlib/Status.h"
#include "om_yourlib/Config.h"

namespace om_yourlib {

class YourLib {
public:
  Status begin(const Config& cfg);
  void end();

  // Cooperative; call often from loop()
  void tick(uint32_t now_ms);

  bool started() const { return started_; }

private:
  Config cfg_{};
  bool started_ = false;
  uint32_t next_ms_ = 0;
};

} // namespace om_yourlib
