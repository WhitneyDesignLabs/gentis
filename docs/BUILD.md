# Build — assembly, flashing, first-boot

Phase-1 (brain-only) build & bring-up for the Gentis ESP32-C6. Mechanical assembly and the
Gentis expression/safety layer are covered by later directives; this page covers flashing
the firmware and watching it boot.

## Toolchain

- [PlatformIO Core](https://platformio.org/) (tested with 6.1.19). Invoke as `pio` or,
  if `pio` isn't on PATH, `python -m platformio`.
- The ESP32 toolchain (pioarduino platform, RISC-V GCC, esptool) is fetched automatically
  on the first `pio run`.

## Board & USB ports (important)

The target is an **ESP32-C6-WROOM-1-N16** (16 MB). The DevKitC-1-style board exposes the
C6's **native USB-Serial-JTAG** port — flash *and* monitor on that one port. Firmware is
built so Arduino `Serial` goes to this native USB port — `firmware/platformio.ini` sets
**both** `-D ARDUINO_USB_CDC_ON_BOOT=1` and `-D ARDUINO_USB_MODE=1` (the C6 needs both: the
first enables CDC-on-boot, the second selects the hardware USB-Serial-JTAG `HWCDCSerial`
rather than a TinyUSB device the C6 doesn't have). So you do **not** need the separate
"UART" connector.

- **Monitor baud:** `115200`.
- The committed `upload_port`/`monitor_port = /dev/ttyACM0` in `platformio.ini` are
  placeholders. **Pin the real port per command** (on Windows, e.g. `COM3`).

## Windows: force UTF-8 (required)

On Windows consoles, `pio` uploads can crash while echoing esptool's Unicode output
(`UnicodeEncodeError: 'charmap' codec ...`). Export UTF-8 before running `pio`:

```sh
# PowerShell
$env:PYTHONUTF8 = "1"; $env:PYTHONIOENCODING = "utf-8"
# bash
export PYTHONUTF8=1 PYTHONIOENCODING=utf-8
```

## Flashing — select the target BY CHIP, then pin the port

If more than one ESP32 is plugged in (e.g. a C6 here, plus an S3 STT-dongle), **never**
auto-detect the upload port — you could flash the wrong board. Identify the C6 by chip type
and pin that port:

```sh
# 1. List serial ports
pio device list

# 2. For each Espressif port (USB VID 303A), confirm the chip. The C6 reports "ESP32-C6".
#    (esptool ships inside PlatformIO; use the penv copy or `python -m esptool` if installed.)
esptool --port COM3 flash_id        # -> "Detecting chip type... ESP32-C6", "flash size 16MB"

# 3. Build + upload the app to the pinned C6 port. esptool re-checks the chip and aborts on
#    mismatch — do NOT pass --force / never override the chip check.
pio run -e esp32-c6 -t upload --upload-port COM3

# 4. (only if the filesystem/partitions changed) upload the LittleFS image:
pio run -e esp32-c6 -t uploadfs --upload-port COM3
```

> Safety rule: target by `chip == ESP32-C6` and pin `--upload-port`. Never point
> `--upload-port` at an S3 dongle; never use auto-detect for upload.

## First boot & monitor

```sh
pio device monitor -p COM3 -b 115200
```

Expect a clean boot, the WireClaw banner, and — with no WiFi configured — the captive-portal
Wi-Fi AP **`WireClaw-Setup`** (WireClaw's default name; the `Gentis-Setup` rename is a later
directive). A stable, advertised AP means a healthy boot (no boot-loop/brownout).

> A note on the native USB-Serial-JTAG console: the USB port re-enumerates on reset, so a
> monitor may briefly drop and reconnect when the board resets — that's normal.

## First-run provisioning (manual)

Join the `WireClaw-Setup` AP from a phone/laptop; the captive portal opens. Enter your WiFi
credentials and an OpenRouter free-tier API key, then Save & Reboot. (Secrets are entered by
the operator here — they are never committed to the repo.)
