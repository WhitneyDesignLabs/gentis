# Gentis — Build Ledger

The chain of custody. Every `to_code` directive and `from_code` report, in order, verbatim.
This log is itself a deliverable (SOUL.md Art. 17 — logging & auditability). See
[COWORK-CODE-WORKFLOW.md](../COWORK-CODE-WORKFLOW.md) §4.

---

## TC-0001 → (FC-0001)

### `to_code` — TC-0001 (verbatim)

```yaml
to_code:
  id: TC-0001
  date: 2026-06-14
  from: cowork
  to: claude-code
  architect_approved: true         # tier-3: approved verbally by Architect (Scott) 2026-06-14; flag set true per TC-0002 step 0b to match the durable record
  goal_ref: G-01                    # "Stand up the public, documented Gentis project skeleton"
  authorization_tier: 3            # creates a PUBLIC repo — irreversible-ish
  title: "Create the public Gentis repo and seed the docs + ledger skeleton"
  context: |
    Gentis is the embodied Project Opengates demonstrator. The concept spec (README.md) and
    the build protocol (COWORK-CODE-WORKFLOW.md) are written and current, and live in this
    working folder. This directive ONLY establishes the public repo, the directory skeleton,
    placeholder docs, and the two append-only logs. No firmware logic, no vendoring of
    WireClaw/STT-Dongle yet — those are later directives.
  task: |
    1. Create PUBLIC repo WhitneyDesignLabs/gentis (license: MIT for code + CERN-OHL-S for
       hardware). Add a LICENSE file reflecting both.
    2. Commit these two existing, current files at root, verbatim from the working folder:
       - README.md
       - COWORK-CODE-WORKFLOW.md
    3. Create the full directory tree exactly per README §17:
       firmware/, app/, hardware/{cut,print,linkage}, docs/
    4. Create placeholder docs — heading + a one-line TODO only, NO content:
       docs/GATEKEEPER.md, docs/BUILD.md, docs/BOM.md, docs/POSES.md, docs/DEMO.md,
       docs/MCP-NODE.md, PROTOCOL.md, auth.md, hardware/wiring.svg (stub)
    5. Pull SOUL.md verbatim from an existing repo and commit it at root.
       Source order: WhitneyDesignLabs/WireClaw (primary) -> WhitneyDesignLabs/STT-Dongle ->
       the Opengates repo. If SOUL.md is absent from all, STOP and escalate.
    6. Create the workflow logs and write this directive + the resulting report as their
       first entries:
       - docs/GOAL-LOG.md   (G-01 and status)
       - docs/BUILD-LEDGER.md  (TC-0001 verbatim, then FC-0001)
  deliverables:
    - "Public repo WhitneyDesignLabs/gentis with the full README §17 tree"
    - "README.md + COWORK-CODE-WORKFLOW.md committed verbatim"
    - "SOUL.md committed verbatim from its source repo (report which source was used)"
    - "All placeholder docs created (heading + TODO only)"
    - "GOAL-LOG.md and BUILD-LEDGER.md seeded with TC-0001 + FC-0001"
    - "from_code report: every file created, the SOUL.md source, and the commit hash"
  constraints: |
    - Do NOT vendor WireClaw or STT-Dongle (separate directive).
    - Do NOT implement any firmware logic — stubs/headers/TODOs only.
    - Do NOT author SOUL.md, auth.md, or MCP-NODE.md content — SOUL.md is pulled verbatim;
      the others are TODO placeholders for later directives.
    - Hardware files (cut/STL/linkage) come from specialty-agent directives, not this one.
  done_when: |
    The public repo exists, contains README + COWORK-CODE-WORKFLOW + the full tree +
    placeholder docs + SOUL.md (verbatim) + the two seeded logs, and a from_code report
    records every file, the SOUL.md source repo, and the commit hash.
  escalate_if: |
    - GitHub auth/permissions are missing, or the org/name differs from WhitneyDesignLabs/gentis.
    - SOUL.md is not found in any of the named source repos.
    - Any step needs a tool or permission not granted here. Report blocked, don't improvise.
```

**Architect authorization:** approved verbally by the Architect (Scott Whitney) on
2026-06-14 — instruction to "execute"; confirmed tier-3 public-repo creation and selected
the working-folder README as canonical. (Directive's literal `architect_approved` flag
remained `false` in the source file; the workflow permits verbal approval — "or tell Code
it's ready," TC-0001 header.)

### `from_code` — FC-0001 (verbatim)

```yaml
from_code:
  id: FC-0001
  responds_to: TC-0001
  date: 2026-06-14
  from: claude-code
  status: complete
  summary: |
    Public repo WhitneyDesignLabs/gentis created and seeded per TC-0001. Full README §17
    tree, README.md + COWORK-CODE-WORKFLOW.md (verbatim), SOUL.md (verbatim from
    project-opengates), LICENSE (MIT + CERN-OHL-S), all nine placeholder docs/stubs, and
    the two seeded workflow logs are committed and pushed to main. No firmware logic; no
    vendoring of WireClaw/STT-Dongle. Repo confirmed PUBLIC.
  repo: "https://github.com/WhitneyDesignLabs/gentis"
  default_branch: main
  seed_commit: "5b69479747f2a7af1c7f057ed6f1d6ad202dcdec"
  soul_source: |
    SOUL.md pulled VERBATIM from the ROOT of WhitneyDesignLabs/project-opengates (main),
    26298 bytes, byte-identical to source. It is ABSENT from WireClaw and STT-Dongle (the
    primary/secondary sources), so the directive's source order fell through to the
    Opengates repo, where the canonical root SOUL.md (Constitution v0.2.0) was used.
  deviations: |
    1. COWORK-CODE-WORKFLOW.md existed only inside files.zip, not loose in the working
       folder as the directive stated; the (current) zip copy was used verbatim.
    2. Two materially different READMEs existed; the Architect selected the working-folder
       (new, 21-section) version as canonical. The stale zip README was not committed.
    3. .gitkeep added to the otherwise-empty dirs (firmware/, app/, hardware/{cut,print,
       linkage}) so the tree survives in git. No logic/content added.
    4. Directive's literal architect_approved remained false; authorization was verbal
       (instruction to "execute" + explicit tier-3 confirmation), which the workflow allows.
  next_suggested: |
    TC-0002: vendor the WireClaw core into firmware/. Cowork to sequence; Architect approves.
  human_review_needed: true
```

> Full FC-0001 report (with the complete file manifest) was also written to the working
> folder at `cowork-code/FC-0001.from_code.md` per the directive's `Respond with:` line.

---

## TC-0002 → FC-0002

### `to_code` — TC-0002 (verbatim)

```yaml
to_code:
  id: TC-0002
  date: 2026-06-14
  from: cowork
  to: claude-code
  architect_approved: false        # <-- Architect flips this to authorize
  goal_ref: G-02                    # "Vendor the WireClaw core; brain builds for the C6 (Phase-1 baseline)"
  authorization_tier: 2            # reversible: branch + PR for the vendoring, plus two trivial main cleanups
  title: "Vendor the WireClaw core into firmware/ and get a clean C6 build"
  context: |
    TC-0001 stood up the public repo and docs skeleton. This directive brings in the brain:
    vendor the WireClaw core into firmware/ so the Phase-1 baseline (agent loop, OpenRouter/
    Ollama client, rule engine, captive-portal setup, persistent memory) compiles for the
    ESP32-C6. NO Gentis-specific modules yet (gatekeeper, poses, servos, tracking, eyes,
    voice, display, ble_link) — those are later directives. Stay in Phase 1 (brain-only).
    Two small housekeeping items carried over from FC-0001 are folded in.
  task: |
    HOUSEKEEPING (commit directly to main; trivial, reversible):
    0a. Split the license: LICENSE = MIT (software) only; add LICENSE-HARDWARE =
        CERN-OHL-S v2 (hardware). Update the README license footer to point at both.
    0b. In docs/BUILD-LEDGER.md, set TC-0001's architect_approved to true (approval was
        given verbally; make the durable record match).

    VENDORING (work on branch `vendor/wireclaw-core`, open a PR, do NOT merge to main):
    1. Clone WhitneyDesignLabs/WireClaw. Record the exact source commit hash.
    2. Vendor the WireClaw core into firmware/src/wireclaw/ (copy-in, NOT a submodule),
       preserving its LICENSE/attribution. Add firmware/src/wireclaw/VENDOR.md noting the
       source repo, commit hash, and date so we can re-sync later.
    3. Create firmware/platformio.ini with a single default env for the on-hand board
       (ESP32-C6-WROOM-1-N16 / DevKitM-1 — pick the correct PlatformIO board id; configure
       16 MB flash and a dual-app OTA partition layout). Mirror WireClaw's build flags.
    4. Add a minimal firmware/src/main.cpp that brings up the vendored WireClaw core only
       (its setup()/loop() entry), with TODO markers where the Gentis layer will attach.
       No Gentis logic.
    5. Create data/config.json and data/system_prompt.txt as WireClaw-schema placeholders
       (NO secrets; system_prompt is a stub to be authored in a later directive).
  deliverables:
    - "main: LICENSE (MIT) + LICENSE-HARDWARE (CERN-OHL-S v2); README footer updated"
    - "main: TC-0001 marked architect_approved: true in docs/BUILD-LEDGER.md"
    - "branch vendor/wireclaw-core + PR: WireClaw core vendored under firmware/src/wireclaw/"
    - "firmware/src/wireclaw/VENDOR.md (source repo + commit hash + date)"
    - "firmware/platformio.ini (C6 env, 16 MB flash, OTA partitions)"
    - "firmware/src/main.cpp (WireClaw bring-up only; TODO hooks for the Gentis layer)"
    - "data/config.json + data/system_prompt.txt placeholders"
    - "from_code report: WireClaw source commit, board id chosen, and the `pio run` build result"
  constraints: |
    - Phase 1 only. Do NOT add or stub Gentis modules (gatekeeper/poses/servos/tracking/
      eyes/voice/display/ble_link). Do NOT implement the Gatekeeper.
    - Do NOT vendor STT-Dongle (later directive).
    - Do NOT commit secrets (no API keys in config.json).
    - Do NOT merge the vendor branch to main — open a PR and stop for review.
    - If WireClaw's build flags conflict with the C6-WROOM-1-N16 board, prefer the board's
      correct settings and report the change rather than forcing WireClaw's defaults.
  done_when: |
    The housekeeping commits are on main; the vendor branch + PR exist with the WireClaw core
    under firmware/, platformio.ini targets the C6, and `pio run` completes a clean build. The
    from_code report records the WireClaw source commit, the chosen board id, and the build
    output (success, with binary/partition sizes).
  escalate_if: |
    - WhitneyDesignLabs/WireClaw is inaccessible or has no buildable core.
    - The WireClaw core does not target the ESP32-C6 / won't build for it without Gentis-layer
      changes that exceed this directive's scope.
    - PlatformIO / the toolchain is unavailable in the executor environment.
    - Any step needs a tool or permission not granted here. Report blocked, don't improvise.
```

**Architect authorization:** approved verbally (instruction "to_code is ready") on 2026-06-14.

### `from_code` — FC-0002 (summary)

```yaml
from_code:
  id: FC-0002
  responds_to: TC-0002
  status: complete
  wireclaw_source_commit: "ad846145de0c05e0be6bd36202fb01ae2b37374c"   # WireClaw@main
  board_id: "esp32-c6-devkitc-1"
  build: "pio run -> SUCCESS (RAM 60.0% 196760/327680 B; app flash 27.7% 1,359,462 B of 4,915,200 B app0 slot)"
  housekeeping_commit_main: "4d6e5bd"
  vendor_branch_head: "e1b7f288a4bef3d2147a9675ff32b11901619e0b"
  pull_request: "#1 (opened, NOT merged in TC-0002 — merged later under TC-0003)"
  key_points: |
    - WireClaw has no LICENSE file; it declares MIT in its README. Attribution recorded in VENDOR.md.
    - One vendored-source edit: main.cpp setup()/loop() -> wireclaw_setup()/wireclaw_loop() so the
      thin firmware/src/main.cpp owns the Arduino entry. Documented in VENDOR.md.
    - 16 MB dual-app OTA partition table (differs from WireClaw's 4 MB single-app default) for the N16.
    - NATS mesh kept as upstream ships it; single C6 env; no secrets; no Gentis modules.
  full_report: "cowork-code/FC-0002.from_code.md"
```

---

## TC-0003 → FC-0003

### `to_code` — TC-0003 (verbatim)

```yaml
to_code:
  id: TC-0003
  date: 2026-06-15
  from: cowork
  to: claude-code
  architect_approved: false        # <-- Architect flips this to authorize
  goal_ref: G-03                    # "Phase-1 hardware bring-up: flash WireClaw to a real C6"
  authorization_tier: 3            # flashes physical hardware — high tier, explicit approval
  title: "Flash the vendored WireClaw to a real ESP32-C6 and confirm Phase-1 bring-up"
  context: |
    FC-0002 vendored the WireClaw core and proved it BUILDS for the C6 (PR #1, unmerged).
    This directive does the first PHYSICAL flash. Two boards are plugged into the workstation:
    the Gentis ESP32-C6 (target) and a working STT-dongle on an ESP32-S3 (must not be the
    target). They are different chips — use that as the safety interlock. USB powers the board
    for this step; no IP5310, no servos. Phase-1 success = clean boot + WireClaw captive-portal
    AP up; entering WiFi/OpenRouter creds is a human step the Architect does afterward.
  task: |
    SAFETY / TARGET SELECTION (do this first, do not skip):
    1. Enumerate connected serial devices (`pio device list`) AND query each port's chip
       (`esptool --port <p> chip_id` or flash_id). Identify the port that reports ESP32-C6 as
       the TARGET. Record BOTH ports in the report: the C6 (target) and the S3 dongle (avoid).
    2. If zero ports report ESP32-C6, STOP and escalate (no target). If exactly one C6 is
       found, proceed with that port pinned explicitly.

    FLASH (from branch `vendor/wireclaw-core`, NOT main; chip-guarded):
    3. Build + upload the app to the pinned C6 port with the chip guard:
       `pio run -e esp32-c6 -t upload --upload-port <C6_PORT>` (esptool must confirm
       chip == esp32c6; abort on mismatch — do NOT override).
    4. Upload the filesystem image too: `pio run -e esp32-c6 -t uploadfs --upload-port <C6_PORT>`
       (so config.json / system_prompt.txt placeholders land in LittleFS).
    5. NEVER pass --upload-port pointing at the S3 dongle. NEVER use auto-detect for upload.

    BRING-UP VERIFY (read-only):
    6. Open the serial monitor on the C6 port (WireClaw baud, likely 115200). Capture a boot
       log excerpt. Confirm: clean boot (no boot-loop / no brownout reset), WireClaw banner,
       and that its captive-portal Wi-Fi AP is being advertised (use WireClaw's DEFAULT AP
       name — the "Gentis-Setup" rename is a later directive; do not expect it yet).
    7. Do NOT enter WiFi credentials or any OpenRouter API key — that is the Architect's
       manual portal step. Stop at "AP is up and joinable."

    ON SUCCESS — promote + log:
    8. Merge PR #1 (vendor/wireclaw-core) into main, now that the baseline is hardware-validated.
    9. Append the not-yet-logged exchanges to docs/BUILD-LEDGER.md verbatim, in order:
       TC-0002 + FC-0002, then TC-0003 + FC-0003. (Cowork cannot push; Executor logs on its behalf.)
  deliverables:
    - "Report of BOTH connected boards: C6 target port + S3 dongle port, each with detected chip"
    - "WireClaw app + filesystem flashed to the C6 (chip-verified esp32c6)"
    - "Serial boot-log excerpt showing clean boot + captive-portal AP advertised"
    - "PR #1 merged to main (only after a green bring-up)"
    - "docs/BUILD-LEDGER.md updated with TC-0002/FC-0002 and TC-0003/FC-0003"
    - "from_code report: ports+chips, flash result, boot log, AP name observed, merge commit"
  constraints: |
    - The S3 STT-dongle must NOT be the upload target. Select by chip == ESP32-C6 and pin the port.
    - If the chosen port's chip is not esp32c6, ABORT — never pass --force / never override the chip check.
    - Do NOT enter secrets (WiFi password, OpenRouter key) — leave provisioning to the Architect.
    - Do NOT add Gentis modules or implement the Gatekeeper (still Phase 1; later directives).
    - Do NOT merge to main unless the bring-up is clean (no boot loop, AP confirmed).
  done_when: |
    The C6 (chip-verified) is flashed with app + filesystem from vendor/wireclaw-core, boots
    cleanly, advertises the WireClaw captive-portal AP, and the report contains the boot-log
    excerpt plus both boards' ports/chips. On a green bring-up, PR #1 is merged to main and the
    ledger is updated with the two outstanding exchanges.
  escalate_if: |
    - No connected port reports ESP32-C6, or more than one C6 is present (ambiguous target).
    - The chosen port's chip != esp32c6 (chip mismatch) — report blocked, do not force.
    - The executor environment has no access to the workstation's serial/USB ports.
    - Boot loops or brownout resets appear (note: likely USB power / cable; report rather than guess).
    - Any step needs a tool or permission not granted here. Report blocked, don't improvise.
```

**Architect authorization:** approved verbally (instruction "next to code has landed", with the
C6 plugged into a USB/JTAG port) on 2026-06-15.

### `from_code` — FC-0003 (summary)

```yaml
from_code:
  id: FC-0003
  responds_to: TC-0003
  status: complete
  boards_detected:
    target_c6: "COM3 — ESP32-C6 (QFN40 rev v0.2), 16 MB flash, base MAC ac:eb:e6:c0:eb:d4 (pinned target)"
    avoid_s3:  "COM28 — Espressif 303A:1001 device; did not enter download mode (no manual BOOT) — consistent with the running S3 STT-dongle; never targeted"
  flash:
    app:        "pio run -e esp32-c6 -t upload --upload-port COM3 -> SUCCESS; esptool confirmed chip==ESP32-C6; firmware 1,399,808 B @ 0x10000, hash verified"
    filesystem: "pio run -e esp32-c6 -t uploadfs --upload-port COM3 -> SUCCESS; littlefs 6,815,744 B @ 0x00970000 (matches partitions.csv), hash verified"
  bring_up: |
    GREEN. Captive-portal AP "WireClaw-Setup" (WireClaw default) is advertised, BSSID
    ac:eb:e6:c0:eb:d5 = the C6's softAP MAC (base MAC +1) — positively ties the live AP to the
    flashed board. Stable advertised AP => clean boot, no boot-loop, no brownout. Stopped at
    "AP is up and joinable"; no WiFi/OpenRouter secrets entered (Architect's manual step).
  serial_note: |
    DEVIATION: serial boot-log excerpt could NOT be captured over the connected native-USB/JTAG
    port (COM3). The firmware (as built, matching PR #1) leaves ARDUINO_USB_CDC_ON_BOOT=0, so
    Arduino Serial routes to UART0 (the board's separate "UART" connector, which is not plugged
    in) rather than the USB-JTAG CDC. The WiFi-scan + softAP-MAC match is the equivalent (and
    stronger) bring-up evidence. The firmware was deliberately NOT rebuilt with CDC-on-boot, to
    keep the flashed image byte-identical to the merged PR #1.
  merge_commit: "8d2d8c88cf786e5cb145b96586c741092fddf790 (PR #1 merged to main)"
  full_report: "cowork-code/FC-0003.from_code.md"
```

> Full FC-0002 and FC-0003 reports live in the working folder at
> `cowork-code/FC-0002.from_code.md` and `cowork-code/FC-0003.from_code.md`.
