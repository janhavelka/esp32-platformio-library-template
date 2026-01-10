# esp32-platformio-library-template

A clean, robust **single-library** template for **ESP32 (S2/S3)** using **Arduino framework** with **PlatformIO**.

## Targets
- **ESP32-S3-MINI-1U-N4R2** (PSRAM enabled explicitly in `platformio.ini`)
- **ESP32-S2-MINI-2-N4**

## Build / Upload / Monitor (examples)

### Example CLI (S3)
```bash
pio run -e ex_cli_s3
pio run -e ex_cli_s3 -t upload
pio device monitor -e ex_cli_s3
```

### Example CLI (S2)
```bash
pio run -e ex_cli_s2 -t upload
pio device monitor -e ex_cli_s2
```

## Library API (recommended pattern)
- `Status begin(const Config&)`
- `void tick(now_ms)` (non-blocking, cooperative)
- `void end()`

## Portability rule
The library does **not** own board pins. Pins and parameters are injected via `Config`.
Examples provide defaults under `examples/common/`.
