# esp32-platformio-library-template

A clean, robust **single-library** template for **ESP32 (S2/S3)** using **Arduino framework** with **PlatformIO**.

[![CI](https://github.com/YOUR_USERNAME/esp32-platformio-library-template/actions/workflows/ci.yml/badge.svg)](https://github.com/YOUR_USERNAME/esp32-platformio-library-template/actions/workflows/ci.yml)

## Quickstart

```bash
# Clone
git clone https://github.com/YOUR_USERNAME/esp32-platformio-library-template.git
cd esp32-platformio-library-template

# Build for ESP32-S3
pio run -e ex_cli_s3

# Upload and monitor
pio run -e ex_cli_s3 -t upload && pio device monitor -e ex_cli_s3
```

## Supported Targets

| Board                    | Environment       | Notes                    |
| ------------------------ | ----------------- | ------------------------ |
| ESP32-S3-MINI-1U-N4R2    | `ex_cli_s3`       | PSRAM enabled            |
| ESP32-S2-MINI-2-N4       | `ex_cli_s2`       | No PSRAM                 |

## API

The library follows a **begin/tick/end** lifecycle:

```cpp
#include "om_yourlib/YourLib.h"

om_yourlib::YourLib lib;

void setup() {
  om_yourlib::Config cfg;
  cfg.led_pin = 48;
  cfg.interval_ms = 1000;

  om_yourlib::Status st = lib.begin(cfg);
  if (!st.ok()) {
    // Handle error: st.code, st.msg, st.detail
  }
}

void loop() {
  lib.tick(millis());  // Non-blocking, call every iteration
}

// Optional cleanup
void shutdown() {
  lib.end();
}
```

### Core Methods

| Method                          | Description                              |
| ------------------------------- | ---------------------------------------- |
| `Status begin(const Config&)`   | Initialize with configuration            |
| `void tick(uint32_t now_ms)`    | Cooperative update, call from `loop()`   |
| `void end()`                    | Stop and release resources               |
| `bool started() const`          | Check if library is active               |

## Config

Configuration is injected via `Config` struct. The library **never hardcodes pins**.

```cpp
struct Config {
  int led_pin = -1;           // GPIO for LED (-1 = disabled)
  int uart_rx_pin = -1;       // Example: UART RX pin
  int uart_tx_pin = -1;       // Example: UART TX pin
  uint32_t interval_ms = 1000; // Periodic tick interval
};
```

See [include/om_yourlib/Config.h](include/om_yourlib/Config.h) for full definition.

### Pin Mapping

The library **does not define pin defaults**. All pins are application-provided via `Config`.

For convenience, examples use reference pin mappings defined in [examples/common/BoardPins.h](examples/common/BoardPins.h):

| Signal    | GPIO | Note                               |
| --------- | ---- | ---------------------------------- |
| SDA       | 8    | I2C data line                      |
| SCL       | 9    | I2C clock line                     |
| SPI_MOSI  | 11   | SPI master out, slave in           |
| SPI_SCK   | 12   | SPI serial clock                   |
| SPI_MISO  | 13   | SPI master in, slave out           |
| LED       | 48   | Onboard LED (48=S3, 18=S2 typical) |

**These are example defaults for ESP32-S2 / ESP32-S3 reference hardware only.** Override for your board.

## Error Model

All fallible operations return `Status`:

```cpp
struct Status {
  Err code;           // Error category (Ok, InvalidArg, Timeout, etc.)
  int32_t detail;     // Vendor/library-specific error code
  const char* msg;    // Human-readable message (STATIC STRING ONLY)
};
```

**Important:** `msg` must always point to a static string literal. Never allocate or construct strings dynamically. This ensures zero heap allocation in error paths.

### Error Codes

| Code          | Meaning                              |
| ------------- | ------------------------------------ |
| `Ok`          | Success                              |
| `InvalidArg`  | Invalid configuration parameter      |
| `Timeout`     | Operation timed out                  |
| `Busy`        | Resource is busy                     |
| `Io`          | I/O error                            |
| `NotReady`    | Not initialized or not ready         |
| `NoMem`       | Memory allocation failed             |
| `ThirdParty`  | Error from wrapped third-party code  |
| `Internal`    | Internal logic error                 |

## Threading & Timing Model

- **Non-blocking:** `tick()` returns immediately; no delays in steady state.
- **Single-threaded:** Call all methods from the same task/thread (typically Arduino `loop()`).
- **Cooperative:** You control when work happens by calling `tick()`.
- **Deterministic:** Predictable execution time; no hidden sleeps or waits.

**ISR Safety:** Do not call library methods from ISRs. Set flags in ISRs and handle them in `tick()`.

## Design Notes

This library follows embedded best practices:

1. **Deterministic behavior:** No hidden delays, no unbounded loops.
2. **Non-blocking:** All operations complete quickly or report busy.
3. **Config injection:** Hardware pins and parameters come from `Config`, not hardcoded.
4. **No hidden NVS:** No persistent storage side effects unless explicitly documented and opt-in.
5. **Static error strings:** `Status.msg` is always a string literal, never heap-allocated.
6. **No steady-state allocations:** All memory is allocated in `begin()`, none in `tick()`.

## Examples

| Example                  | Description                                      |
| ------------------------ | ------------------------------------------------ |
| `00_compile_only`        | Minimal skeleton; verifies library compiles      |
| `01_basic_bringup_cli`   | Interactive CLI for testing start/stop           |

### Building Examples

```bash
# Compile-only skeleton (S3)
pio run -e ex_compile_only_s3

# CLI example (S2)
pio run -e ex_cli_s2 -t upload
pio device monitor -e ex_cli_s2
```

## Versioning Policy

This project follows [Semantic Versioning 2.0.0](https://semver.org/):

- **MAJOR:** Breaking API changes
- **MINOR:** New features, backward compatible
- **PATCH:** Bug fixes, backward compatible

### Release Checklist

1. Update version in `library.json`
2. Update `CHANGELOG.md` (move Unreleased to new version)
3. Commit: `git commit -m "chore: release v1.2.3"`
4. Tag: `git tag v1.2.3`
5. Push: `git push && git push --tags`

## Project Structure

```
├── include/om_yourlib/   # Public headers (library API)
│   ├── Config.h          # Configuration struct
│   ├── Status.h          # Error types
│   └── YourLib.h         # Main library class
├── src/                  # Implementation
│   └── YourLib.cpp
├── examples/
│   ├── 00_compile_only/  # Minimal skeleton
│   ├── 01_basic_bringup_cli/  # CLI demo
│   └── common/           # Shared example utilities
├── .github/workflows/    # CI configuration
├── library.json          # PlatformIO library metadata
└── platformio.ini        # Build environments
```

## Extending This Template

This template is designed to scale across diverse embedded projects. When adding new functionality:

### Device Integration Patterns

**RS485/Modbus:**
- Use transaction-based state machine (Idle → Tx → Rx → Done)
- Implement inter-character and frame timeouts via deadlines
- Optional: RX drain task for high-throughput scenarios

**GSM Modems / AT Commands:**
- Command queue with retry logic and per-command timeouts
- Handle unsolicited responses (+CMT, +CREG, etc.) in tick()
- Some commands take 30+ seconds - use deadline-based waits

**High-Rate ADC:**
- ISR writes to ring buffer (minimal work)
- Optional processing task drains buffer
- Document buffer size requirements in Config

**Stepper Motors:**
- Non-blocking position tracking API
- Use hardware timers or RMT for pulse generation
- Never block waiting for motion completion

**I2C/SPI Sensors:**
- Short transactions in tick(), avoid long bus holds
- Implement timeout + retry with exponential backoff
- Abstract shared bus ownership if multiple devices

**SD Card Logging:**
- Buffer writes in RAM, flush periodically or on demand
- Optional task for background flushing
- Handle write failures gracefully (retry, report error)

### FreeRTOS Tasks (When to Use)

Default: **Do not use tasks.** Implement as non-blocking tick() pattern.

Use tasks only when:
- Continuous streaming required (ADC sampling, audio)
- Blocking I/O simplifies correctness (UART RX, sockets)

When adding tasks:
- Keep them thin adapters calling library tick()
- Document stack size, priority, and lifecycle in Config
- Provide both task-based AND non-blocking APIs when possible
- Update README threading model section

### Modification Checklist

Before extending:
1. Does it increase predictability? (If no, reconsider)
2. Add Doxygen docs to all new public APIs
3. Update README with threading/timing impacts
4. Keep Config struct board-agnostic (no hardcoded pins)
5. Ensure tick() remains bounded and non-blocking
6. Add entry to CHANGELOG.md

## Assumptions

- Target boards have at least 4MB flash.
- Arduino framework provides `millis()` returning `uint32_t`.
- Examples assume onboard LED on GPIO 48 (S3) - adjust in `BoardPins.h`.
- Single-threaded by default; tasks are opt-in via Config.

## Contributing

See [CONTRIBUTING.md](CONTRIBUTING.md) for guidelines.

## License

This project is licensed under the MIT License - see [LICENSE](LICENSE) for details.

## See Also

- [CHANGELOG.md](CHANGELOG.md) - Version history
- [SECURITY.md](SECURITY.md) - Security policy
- [AGENTS.md](AGENTS.md) - AI agent guidelines
