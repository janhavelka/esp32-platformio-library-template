#pragma once
#include <stdint.h>

namespace om_yourlib {

enum class Err : uint16_t {
  Ok = 0,
  InvalidArg,
  Timeout,
  Busy,
  Io,
  NotReady,
  NoMem,
  ThirdParty,
  Internal
};

struct Status {
  Err code = Err::Ok;
  int32_t detail = 0;       // vendor/lib error code if relevant
  const char* msg = "";     // static string only (no heap)

  constexpr bool ok() const { return code == Err::Ok; }
};

constexpr Status Ok() { return {Err::Ok, 0, ""}; }

} // namespace om_yourlib
