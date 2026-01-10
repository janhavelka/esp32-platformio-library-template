/**
 * @file Status.h
 * @brief Error handling types for YourLib.
 *
 * Provides a lightweight, zero-allocation error model. All error messages
 * are static string literals.
 */

#pragma once

#include <stdint.h>

namespace om_yourlib {

/**
 * @brief Error code enumeration.
 *
 * Covers common embedded error scenarios. When wrapping third-party libraries,
 * translate their errors to the most appropriate Err code and store the
 * original error in Status::detail.
 */
enum class Err : uint16_t {
  Ok = 0,       ///< Success, no error
  InvalidArg,   ///< Invalid argument or configuration
  Timeout,      ///< Operation timed out
  Busy,         ///< Resource is busy, try again later
  Io,           ///< I/O or communication error
  NotReady,     ///< Not initialized or prerequisite not met
  NoMem,        ///< Memory allocation failed
  ThirdParty,   ///< Error from wrapped third-party library
  Internal      ///< Internal logic error (bug)
};

/**
 * @brief Operation result with error details.
 *
 * Returned by fallible operations. Check with ok() or inspect code/msg.
 *
 * @note The msg field MUST point to a static string literal. Never assign
 *       dynamically allocated strings. This ensures zero heap allocation
 *       in error paths and safe usage across function boundaries.
 *
 * Example:
 * @code
 * Status st = lib.begin(cfg);
 * if (!st.ok()) {
 *   Serial.printf("Error: %s (code=%d, detail=%ld)\n",
 *                 st.msg, (int)st.code, (long)st.detail);
 * }
 * @endcode
 */
struct Status {
  Err code = Err::Ok;       ///< Error category
  int32_t detail = 0;       ///< Vendor/library-specific error code (optional)
  const char* msg = "";     ///< Human-readable message (STATIC STRING ONLY)

  /**
   * @brief Check if operation succeeded.
   * @return true if code == Err::Ok
   */
  constexpr bool ok() const { return code == Err::Ok; }
};

/**
 * @brief Create a success Status.
 * @return Status with Err::Ok
 */
constexpr Status Ok() { return {Err::Ok, 0, ""}; }

}  // namespace om_yourlib
