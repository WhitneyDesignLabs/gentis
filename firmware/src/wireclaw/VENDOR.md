# Vendored: WireClaw core

This directory is a **copy-in vendoring** (not a git submodule) of the WireClaw
core — the agent loop, LLM client (OpenRouter / local Ollama), rule engine,
captive-portal web setup, persistent memory, and the optional NATS mesh layer.

## Source

| | |
|---|---|
| **Repo** | https://github.com/WhitneyDesignLabs/WireClaw |
| **Commit** | `ad846145de0c05e0be6bd36202fb01ae2b37374c` |
| **Branch** | `main` |
| **Vendored on** | 2026-06-14 |
| **Vendored by** | TC-0002 (Gentis build ledger) |

## License / attribution

WireClaw declares **MIT** in its `README.md` (`## License` → "MIT"). The upstream
repo at the vendored commit contains **no standalone `LICENSE` file**; the MIT
declaration in its README is the authoritative statement. Gentis's own software
is likewise MIT (see the repo-root `LICENSE`), so the vendored code is covered
consistently. Attribution to WireClaw / Whitney Design Labs is retained here.

## What was copied

- `src/*.cpp` → `firmware/src/wireclaw/*.cpp`
  (devices, llm_client, main, nats_hal, rules, setup_portal, tools, web_config)
- `include/*.h` → `firmware/src/wireclaw/*.h`
  (devices, llm_client, nats_hal, rules, setup_portal, tools, version, web_config)
  — co-located with the sources so WireClaw's quote-includes (`#include "devices.h"`)
  resolve by same-directory lookup with no build-flag changes.
- `lib/nats/` → `firmware/lib/nats/` (PlatformIO auto-discovered local library)

## Modifications to upstream (kept minimal for easy re-sync)

Exactly **one** source file was edited, and only to relocate the Arduino entry
points so the Gentis firmware entry (`firmware/src/main.cpp`) can own them:

- `firmware/src/wireclaw/main.cpp`:
  - `void setup()`  → `void wireclaw_setup()`
  - `void loop()`   → `void wireclaw_loop()`
  (Both edits are marked inline with a `VENDOR EDIT:` comment. No logic changed.)

Two **new, non-upstream** files were added alongside the vendored code:

- `wireclaw_core.h` — declares `wireclaw_setup()` / `wireclaw_loop()` for the shim.
- `VENDOR.md` — this file.

## Build configuration (differs from upstream on purpose)

WireClaw's upstream `platformio.ini` targets 4 MB flash with a single-app
partition table. Gentis targets the on-hand **ESP32-C6-WROOM-1-N16 (16 MB)** with
a **dual-app OTA** table — see `firmware/platformio.ini` and
`firmware/partitions.csv`. The board id (`esp32-c6-devkitc-1`) and the pioarduino
platform match WireClaw's proven C6 build.

## Re-sync notes

To update WireClaw later: re-copy `src/` + `include/` + `lib/nats/` from a newer
commit, re-apply the two `setup()`/`loop()` renames (or diff against the
`VENDOR EDIT:` markers), and bump the commit hash above.
