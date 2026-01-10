# AGENTS.md

## Role
You are a professional embedded software engineer.

Primary goals:
- Robustness
- Stability
- Deterministic behavior

Target: ESP32-S2 / ESP32-S3, Arduino framework, PlatformIO.

These rules are binding.

---

## Repository Model (Single Library)
This repository is a SINGLE reusable library.

### Folder structure (keep it boring and predictable)
- `include/`: public headers (the library API). No board-specific pin values here.
- `src/`: library implementation.
- `examples/`: small applications that demonstrate usage (CLI-style). May contain `examples/common/` helpers.
- `platformio.ini`: builds examples via `build_src_filter` (select which example to compile).

Rules:
- The library MUST be usable in both small sketches and larger applications.
- The library MUST be board-agnostic:
  - Do not hardcode pins in the library.
  - Pins/interfaces come from a `Config` struct passed to `begin()`.
- The library MUST be non-blocking:
  - Provide `tick(now_ms)` for cooperative use.
  - Avoid delays and long blocking calls in steady paths.
- Persistent storage (NVS) MUST be explicit:
  - No hidden storage side effects.
  - If storage is needed, make it opt-in and documented.

---

## Error Handling
- Library APIs return a small stable Status/Err type owned by this library.
- If wrapping third-party libs, translate their errors into Status.
- Silent failure is unacceptable.

---

## Macros
Macros must not be used for constants (pins, sizes, limits, math).
Use `constexpr`.
Macros are allowed only for conditional compilation and diagnostics.

---

## Memory & Determinism
- No repeated heap allocations in steady-state operation.
- Avoid dynamic allocation unless justified.
- Prefer explicit state machines and predictable timeouts.

---

## Logging
- The library should not require a logging framework.
- Examples may use Serial logging helpers under `examples/common/`.
- Do not log from ISRs.

---

## Decision Rule
Prefer the simplest correct solution.
If it reduces predictability, do not proceed.
