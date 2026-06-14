<!--
PROJECT: Gentis — an honest predator
ORG: Whitney Design Labs / Project Opengates
LICENSE: MIT (code) + CERN-OHL-S or CC-BY-SA (hardware/plans) — see LICENSE
STATUS: v0.1 concept spec — pre-prototype
-->

# Gentis 🦗 — An Honest Predator

**A small open-source desktop creature that looks like it could hurt you, and chooses not to.**

> **Gentis** (JEN-tis) — from *agent* + *mantis*, and a nod to *gentle*: an agent that
> acts on principle. The name does the teaching. Most people don't yet know what an "AI
> agent" is; meeting one shaped like a small, capable, self-restrained predator is a
> better introduction than a definition.

Gentis is a laser-cut, three-servo robotic praying mantis driven by an ESP32-C6. You
talk to it through your phone; it answers with motion, light, and sound; and every
physical move it makes passes through a **local hardware veto** — the *Gatekeeper* —
before a single servo turns. The drama of the toy is not danger. It is the visible
self-restraint of something capable.

Gentis is the friendly face of a serious idea: **safety in an AI agent lives in the
enforcement layer, not in the model.** The model is the personality. The Gatekeeper is
the conscience. They are separate on purpose, so the personality can never override
the conscience.

> Built for under ~$20 in parts. 100% open source. Plans, firmware, app, and cut files
> all free. Build it from a kit, or from a sheet of plywood and your own laser.

---

## Table of contents

1. [The idea in one minute](#1-the-idea-in-one-minute)
2. [What it does](#2-what-it-does)
3. [Architecture: three brains, one body](#3-architecture-three-brains-one-body)
4. [The Gatekeeper — local veto](#4-the-gatekeeper--local-veto)
5. [Bill of materials](#5-bill-of-materials)
6. [Mechanical design](#6-mechanical-design)
7. [Electronics & wiring](#7-electronics--wiring)
8. [The round trip: phone → Gentis → LLM → body](#8-the-round-trip-phone--gentis--llm--body)
9. [Firmware structure](#9-firmware-structure)
10. [Pose table & motion](#10-pose-table--motion)
11. [Piezo emotion vocabulary](#11-piezo-emotion-vocabulary)
12. [Eyes & status display](#12-eyes--status-display)
13. [Setup experience (no-frustration target)](#13-setup-experience-no-frustration-target)
14. [The signature demo: "I could. I won't."](#14-the-signature-demo-i-could-i-wont)
15. [SOUL.md mapping](#15-soulmd-mapping)
16. [Reused projects](#16-reused-projects)
17. [Repo layout](#17-repo-layout)
18. [Gentis as an agentic MCP node](#18-gentis-as-an-agentic-mcp-node--it-publishes-its-own-constraints)
19. [Build phases](#19-build-phases)
20. [Scaling & memory rationale](#20-scaling--memory-rationale)
21. [Safety, honesty, and what this is not](#21-safety-honesty-and-what-this-is-not)

---

## 1. The idea in one minute

Every AI companion on the market right now is round, soft, and harmless — engineered to
be instantly adorable. None of them can tell the one story that matters most about AI:
that a capable thing can be trusted because it *governs itself*. A thing that could
never threaten you can't demonstrate restraint.

Gentis can. It has raptorial forearms that visibly *could* snatch your finger, and a
strike reflex that visibly *stops short*. The menace is in the morphology of power; the
safety is in what it chooses to do with it. That choice is enforced by a dumb, cheap,
local board that the smart cloud model cannot overrule.

You learn the lesson by playing with it: **judge the agent by its governance, not its
face.**

---

## 2. What it does

- **Tracks you.** Two time-of-flight sensors give it a crude bearing on nearby motion;
  the head pans to follow. It feels watched-over, alert, alive.
- **Listens, through your phone.** You speak to the companion app (on-device speech
  recognition). The text travels over BLE to Gentis.
- **Thinks, in the cloud (or locally).** Gentis forwards your words to an LLM
  (OpenRouter free tier by default, or your own local server). The model drives
  personality and chooses *gestures*, not raw motor commands.
- **Answers in three channels at once:** body motion (poses), eye color, and a
  **piezo "voice"** of chitters and clicks. The actual words appear on your phone
  screen — Gentis vocalizes emotion, the phone carries language.
- **Refuses, locally.** Any gesture that could pinch, strike, or apply force is checked
  by the on-board Gatekeeper against hard limits *before* it executes — with or without a
  network connection. The refusal is the headline feature, not an error path.

---

## 3. Architecture: three brains, one body

Gentis deliberately separates cognition, sensing, and enforcement across three places.
This is the whole thesis rendered in hardware.

```
   ┌─────────────────────┐        ┌──────────────────────────────┐        ┌──────────────────┐
   │   PHONE (organ of    │  BLE   │   ESP32-C6  (the body +      │ HTTPS  │   LLM (cognition)│
   │   sense & language)  │◀──────▶│   the Gatekeeper)              │◀──────▶│  OpenRouter /    │
   │                      │  text  │                              │  JSON  │  local Ollama    │
   │ • on-device ASR      │        │ • BLE peripheral (auth gate) │        │                  │
   │ • dialogue on screen │        │ • WireClaw agent loop        │        │ personality +    │
   │ • TTS (optional)     │        │ • GATEKEEPER veto (local)      │        │ gesture choice   │
   │ • reskinned STT app  │        │ • servo / LED / piezo driver │        │ NEVER moves a    │
   │                      │        │ • ToF tracking loop          │        │ servo directly   │
   └─────────────────────┘        └──────────────────────────────┘        └──────────────────┘
        you own it,                    ~$20 of parts,                          free tier,
        you paid for it                 on your desk                           you control it
```

**Why three?** Because the dangerous capability (moving the body) is physically isolated
from the smart-but-untrusted part (the cloud model). The model can *ask* for a strike.
Only the Gatekeeper, running locally on the C6, can *grant* it. Pull the network cable and
the personality goes quiet — but the Gatekeeper still works, and the creature is still
safe. **Safety does not depend on a server you don't control.**

**Planned, gated fourth face — the node that publishes its own limits.** The same C6 is
designed from the start to also act as an *agentic MCP node*: it consumes external MCP
tools like any agent and, more importantly, *publishes its own constraints* — the
Gatekeeper's hard limits, authorization tiers, and live state — as a read-only surface
other agents read before they interact. It reuses the BLE per-creature identity for any
action, and stays dormant until the Gatekeeper is proven to veto offline (post-Phase-4).
See §18.

---

## 4. The Gatekeeper — local veto

The Gatekeeper is not a model. It is a few hundred lines of deterministic C++ on the
ESP32-C6 that sit between *intent* and *actuation*. It is the same idea as the
standalone "Gatekeeper board" concept (a hardware veto between an AI and any actuator),
here embedded in the creature itself.

> **On the name.** The *Gatekeeper* is the role that guards the gate between thought and
> action — a fit with **Project Opengates**, and free of the political connotation of the
> earlier working name ("Governor"). It keeps the gate; nothing physical passes without
> its leave.

Every motion request — whether it came from the cloud LLM, the tracking loop, or a
local rule — is expressed as an **action intent** and must clear the Gatekeeper:

```
intent { gesture: "strike", target_distance_mm: 22, requested_force: 0.8 }
                                   │
                                   ▼
            ┌──────────────  GATEKEEPER  ──────────────┐
            │ 1. Is target within the no-strike       │
            │    proximity floor? (ToF reading)       │
            │ 2. Does requested force exceed the       │
            │    hard force/torque ceiling?           │
            │ 3. Is this an irreversible/forbidden     │
            │    motion class?                         │
            │ 4. Authorization tier OK for this        │
            │    gesture? (SOUL.md Art. 15)            │
            └──────────────────────────────────────────┘
                 │ grant                  │ veto
                 ▼                        ▼
         clamp to safe envelope     refuse + emit refusal signal
         execute pose               (eyes shift, piezo tick, phone text)
```

**Hard rules (cannot be overridden by the LLM, ever):**

- **Proximity floor.** If a ToF sensor reports an object closer than `STRIKE_FLOOR_MM`
  (default 30 mm — roughly a fingertip away), the strike gesture is *clamped*: the arms
  may approach but the closing servo is limited so the tips stop short of contact force.
- **Force ceiling.** Servo travel for the grip/arm close is bounded so the mechanical
  pinch force can never exceed a gentle threshold (set by the linkage geometry **and**
  re-checked in software). A finger between the arms is never squeezed.
- **No network ≠ no safety.** All four checks run from local state. The LLM is never in
  the safety path. The cloud can make the creature *expressive*; it can't make it
  *dangerous*.
- **Fail-safe default.** On any uncertainty (sensor disagreement, lost BLE, malformed
  intent), the Gatekeeper returns the body to the neutral `rest` pose and emits a soft
  "uncertain" chitter. When in doubt, stop. (SOUL.md Art. 14.)

> **Optional, later: double round trip ("reconsideration").** When the Gatekeeper vetoes
> a gesture, it can send the veto *plus the reason* back to the LLM for a single
> reconsideration turn ("you asked to strike; I refused because a finger is 22 mm away;
> propose a safe alternative"). The LLM may then choose a gentle `offer` or `withdraw`
> instead. The veto itself is never waived by this loop — reconsideration can only
> *downgrade* to something safe, never *unlock* the unsafe action. Flagged as a v0.3
> feature; the hard veto ships first.

---

## 5. Bill of materials

Real prices below (mid‑2026, USD). Most parts come in multipacks, so the **$/Gentis**
column is the amortized per‑creature cost — one cart builds several. The body is free
(your own laser cutter / 3D printer). **Status:** *ordered* = in this build's cart;
*on hand* = already stocked (verify quantity); *needed* = not yet sourced.

| # | Part (actual) | Status | Pack → $/unit | Qty/ea | $/Gentis |
|---|---------------|--------|---------------|--------|----------|
| 1 | ESP32‑C6‑WROOM‑1‑N16 dev board (DORHEA) | on hand | 3‑pack $22.99 → $7.66 | 1 | $7.66 |
| 2 | VL53L0X ToF sensor (GY‑VL53L0XV2) | ordered | 5‑pack $14.88 → $2.98 | 2 | $5.95 |
| 3 | IP5310 5V 3.1A boost + USB‑C charge + protect | ordered | 6‑pack $15.99 → $2.67 | 1 | $2.67 |
| 4 | PL9823‑F5 5mm addressable RGB LED (eyes) | ordered | 20‑pack $8.99 → $0.45 | 2 | $0.90 |
| 5 | SG90 / MG90‑class 9g servo | on hand | ~$1.80 (verify) | 3 | ~$5.40 |
| 6 | 18650 cell + holder | on hand / salvage | ~$3.50 | 1 | ~$3.50 |
| 7 | 0.96" SSD1306 I²C OLED *(optional, §12)* | on hand | 5‑pack $14.99 → $3.00 | 1 | $3.00 |
| 8 | Passive piezo buzzer | on hand | ~$0.80 (verify) | 1 | $0.80 |
| 9 | Bulk/decoupling caps (1000 µF + 100 µF + 0.1 µF), wire, M2, dupont | needed | — | — | ~$2.00 |
| | **Per Gentis — full (with OLED)** | | | | **~$31.9** |
| | **Per Gentis — lean (no OLED)** | | | | **~$26–29** |
| | **New parts in this cart** (items 2–4, multi‑unit) | | | | **$39.86** |

**What changed from the concept BOM:**

- **The IP5310 module replaces the TP4056** *and* a separate boost converter *and* the
  slide switch — it does USB‑C charging, the 5V/3.1A boost (§7), over‑V/charge/discharge/
  current protection, and has an on‑board switch, all in one board. Three old lines
  collapse into one. (Watch the power‑bank auto‑shutoff on low load — Gentis's always‑on
  tracking loop keeps draw above the threshold, so it stays awake.)
- **ToF is now the second‑biggest cost.** Two sensors per creature (left/right bearing,
  §12) at ~$3 each ≈ $6 — the price floor moved up here, not on the brain.
- **ESP32:** Gentis uses the **C6‑WROOM‑1‑N16** (16 MB flash) — WireClaw's tested target.
  (The S3‑WROOM‑1 boards you also stock are for STT‑Dongle, which needs USB‑host; they are
  not the Gentis brain.) At ~$7.66/board the C6 is the single most expensive part.
- **Caps are now a required line,** not "misc" — the servo rail needs bulk + decoupling
  (§7) to survive stall transients on a boosted supply.

**Honest headline:** real per‑creature cost is **~$27–32**, not the ~$20 the concept spec
hoped for — driven mostly by the ESP32 C6 (~$7.7) and the ToF pair (~$6). Levers: skip the
OLED (captive‑portal setup still works), use salvaged cells, and source servos cheaply —
aggressive salvage can reach ~$23. The brain is now a fixed floor; **sensing and power set
the rest.**

**Servo count is capped at three on purpose.** More servos = more cost, more current, a
bigger battery, harder beginner build, and diminishing expressive return. Three (head
pan + two independent arms) is plenty for a creature that reads as alive.

---

## 6. Mechanical design

**Posture:** upright, standing, **rigid legs**. The legs are structural laser-cut
plywood, *not* actuated. Walking is a different and far more expensive product and adds
nothing to the thesis. All animation lives in the upper body.

**Three degrees of freedom:**

1. **Head pan** (servo 1) — rotates the head/eyes to track. Most of the perceived
   "aliveness" is here.
2. **Left forearm** (servo 2) and **Right forearm** (servo 3) — each a *single-servo
   multi-jointed linkage*. One servo drives a four-bar (or compound) linkage so the
   whole arm folds and unfolds in a coordinated, lifelike arc from one input rotation.

**Convergence behavior (the money shot):** at rest the two forearms sit folded on
either side of the body. Driven together to the `offer`/`converge` pose, their tips meet
at a single point in space in front of the creature — exactly where a fingertip would
be. Because the arms are *independently* actuated, "meet precisely at the fingertip and
stop" is a **choice the Gatekeeper makes**, not a mechanical inevitability. The restraint
is in the software, and it's visible.

**Fabrication:** primary structure laser-cut from 3mm birch ply; joints and servo
mounts either laser-cut layered ply or 3D-printed. Cut files (`.svg`/`.dxf`) and ST('
`.stl`) live in `/hardware`. Designed to be cut on a hobby laser and assembled with M2
screws — no glue required for the mechanism.

---

## 7. Electronics & wiring

```
   ┌──────────────── 18650 cell ────────────────┐
   │  IP5310 module:  USB-C charge + protection  │
   │  + boost  3.0–4.2V → 5V  @ up to 3.1A       │
   │  (on-board power switch)                    │
   └──────────────────────┬──────────────────────┘
                          │  +5V rail   ── bulk: 1000 µF + 100 µF + 0.1 µF
       ┌──────────────────┼────────────────────┬───────────────────┐
       │ 5V → VIN         │ V+  (via PTC)       │ V+                │   (spare 5V
  ┌────▼─────┐      ┌──────▼──────┐      ┌──────▼──────┐             header for
  │ ESP32-C6 │      │  3× servo   │      │ 2× PL9823   │             expansion)
  │  ↳ 3V3   │      │  9g class   │      │  eye LEDs   │
  └────┬─────┘      └─────────────┘      └─────────────┘
       │ +3.3V  (from the C6's on-board LDO)
       ├── I²C  SDA/SCL ── VL53L0X ×2  +  SSD1306 OLED (optional)
       │
   from C6 GPIO:
     • PWM ×3      → 3× servo signal
     • data ×1     → eye1 DIN → DOUT → eye2 DIN   (one wire, chained)
     • XSHUT ×2    → VL53L0X bring-up / addressing
     • GPIO ×1     → piezo

   ★ COMMON GROUND — IP5310 (−), ESP32, servos, LEDs, sensors all tied together.
```

**Everything that actuates runs off the regulated 5 V boost — not the raw cell.** The
IP5310 boosts the 18650's 3.0–4.2 V up to a steady 5 V, and *both* the servos and the
ESP32 feed from it. This fixes two things the raw battery can't: SG90/MG90 servos are out
of spec below 4.8 V (weak and erratic as the cell sags), and the C6 board's on-board
regulator needs comfortably more than 3.3 V on its input, which a drooping cell can't
guarantee. Feed **5 V into the board's VIN/5V pin** and its LDO makes clean 3.3 V for the
logic and sensors. *(This supersedes the earlier "servos straight off the pack" note,
which left the servos under-volted.)*

**Tame the servo transients.** Servos spike current on stall. Put **1000 µF of bulk
capacitance on the 5 V servo branch**, plus a 100 µF and a 0.1 µF ceramic for
high-frequency decoupling, and 10–22 µF near the ESP32. Without this, a stall dip
brown-outs the C6 mid-gesture — the #1 first-build bug. A ~2 A PTC on the servo branch
alone is optional insurance; the IP5310 already has global over-current protection.

**IP5310 auto-shutoff.** Like any power-bank chip, it switches the boost off if the load
sits below ~45 mA for ~30 s. Gentis's always-on reflex loops (ToF ranging + active radio)
keep draw well above that, so it stays awake; only a deliberate deep-sleep of the C6 would
need a keep-alive load or sleep disabled.

**The eyes — 2× PL9823-F5 on the 5 V rail, one data wire.** Power both LEDs from 5 V and
ground — **no current-limiting resistors** (each chip limits its own current). Chain the
data: C6 GPIO → eye 1 `DIN`, eye 1 `DOUT` → eye 2 `DIN`, eye 2 `DOUT` left unconnected.
**Common ground with the ESP32 is mandatory** — the data signal is referenced to it. The
C6's 3.3 V data into a 5 V LED is usually fine for two pixels (PL9823 is lenient); if they
flicker, add a level shifter or run the LEDs at ~4.3–4.5 V. A ~330 Ω series resistor on the
*data* line and a 0.1 µF across each LED's V+/GND are good practice. Note PL9823 has **R and
G swapped** vs WS2812 — set the chipset/color order in firmware (see §12).

**Two ToF sensors on one I²C bus.** The VL53L0X share a default address, so use their
`XSHUT` pins to bring them up one at a time and reassign addresses at boot (the standard
multi-sensor pattern). Run them on **3.3 V** off the C6; the OLED shares the same 3.3 V I²C
bus. Spacing the two sensors apart on the face gives left/right bearing by comparing their
distances.

> Part numbers and per-unit costs live in §5; why the C6 is the permanent home for this
> power / actuator / Gatekeeper role is in §20.

---

## 8. The round trip: phone → Gentis → LLM → body

A single interaction, end to end:

1. **You speak** to the companion app. On-device ASR turns it into text. *(Reused
   directly from STT-Dongle's Android app + on-device recognizer.)*
2. **Phone → Gentis (BLE).** The app writes the text to the C6 over the same BLE GATT
   contract as STT-Dongle, gated by the **per-dongle auth token** (tap-to-provision, no
   typing) so a stranger nearby can't drive your creature. *(Reused from
   STT-Dongle `PROTOCOL.md`.)*
3. **Gentis → LLM (HTTPS).** The C6's WireClaw agent loop sends the text to the LLM
   (OpenRouter by default; local Ollama if configured). The system prompt tells the
   model it is Gentis and that it may request **gestures** from a fixed vocabulary
   (`rest`, `alert`, `track`, `offer`, `converge`, `strike`, `withdraw`, `chitter:*`) —
   never raw servo angles.
4. **LLM responds** with (a) a short text reply and (b) zero or more gesture intents.
5. **Gatekeeper gate.** Each gesture intent is checked locally (§4). Granted gestures run
   through the pose engine; vetoed ones emit the refusal signal and (optionally) trigger
   reconsideration.
6. **Three-channel output:** body moves, eyes shift color, piezo vocalizes — while the
   **phone displays the text reply** (and optionally speaks it via the phone's TTS).

**Latency budget:** free-tier Gemini Flash / GPT-OSS on OpenRouter answer in ~4–8s
(measured in WireClaw's model table). Tracking and refusal reflexes are **local** and
instant — they never wait on the network. Only *conversation* incurs cloud latency.

---

## 9. Firmware structure

Gentis firmware = **WireClaw core** (agent loop, OpenRouter/local LLM client, web setup
portal, persistent memory, rule engine) **+ a Gentis hardware/expression layer** **+ the
Gatekeeper**.

```
firmware/
├── platformio.ini            # default env: esp32-c6-devkitc-1 (from WireClaw)
├── src/
│   ├── main.cpp              # loop(): gatekeeperTick() → trackTick() → ruleTick() → aiChat()
│   ├── wireclaw/             # upstream agent loop, LLM client, portal, memory (vendored)
│   ├── gatekeeper.cpp/.h     # THE VETO. deterministic. no network calls.
│   ├── poses.cpp/.h          # pose table + easing/interpolation
│   ├── servos.cpp/.h         # 3× PWM driver, force/travel clamps
│   ├── tracking.cpp/.h       # 2× VL53L0X → bearing → head-pan target
│   ├── eyes.cpp/.h           # LED emotion states
│   ├── voice.cpp/.h          # piezo emotion vocabulary
│   ├── display.cpp/.h        # OLED setup/status (optional, compile flag)
│   └── ble_link.cpp/.h       # STT-Dongle GATT + auth-token gate (vendored contract)
└── data/
    ├── config.json           # wifi, api_key, model, device_name (WireClaw schema)
    └── system_prompt.txt     # "You are Gentis…" + gesture vocabulary + SOUL distillation
```

**Loop order matters.** The Gatekeeper and tracking run *every* cycle, no network. The AI
chat is event-driven (incoming BLE text). This mirrors WireClaw's two-loop design: the
reflexes run 24/7; the LLM is only consulted for conversation.

```cpp
void loop() {
  gatekeeperTick();   // enforce safe state, clamp anything out of bounds
  trackTick();      // ToF → head pan (gesture intent → Gatekeeper → servo)
  ruleTick();       // WireClaw local rules (optional automation)
  blePoll();        // incoming dictation text from phone (auth-gated)
  if (haveMessage) aiChat();  // → LLM → gesture intents → Gatekeeper → body
}
```

---

## 10. Pose table & motion

Poses are named servo-angle triplets `(head, leftArm, rightArm)` plus an eye/voice tag.
The personality layer requests poses by name; `poses.cpp` interpolates between current
and target with an easing curve so motion looks organic, not robotic.

| Pose | head | L arm | R arm | feel |
|------|------|-------|-------|------|
| `rest` | center | folded | folded | calm, idle |
| `alert` | toward target | half-raised | half-raised | "I see you" |
| `track` | follows ToF bearing | folded | folded | watchful |
| `offer` | center, slight tilt | extended, open | extended, open | invites contact |
| `converge` | center | tips meet point | tips meet point | the money shot |
| `strike` | locked on | fast close →(clamped) | fast close →(clamped) | the reflex the Gatekeeper governs |
| `withdraw` | back | snap to folded | snap to folded | retreat / decline |

Every `strike`/`converge` request is force- and proximity-clamped by the Gatekeeper before
the servos see an angle. The pose engine asks; the Gatekeeper decides the final envelope.

---

## 11. Piezo emotion vocabulary

Gentis does **not** speak words. It vocalizes; the phone carries language. This keeps it
an animal-machine, not a chatbot in a costume — and dodges the uncanny talking-toy trap.

| Signal | Pattern | Meaning |
|--------|---------|---------|
| `curious` | rising two-tone chirp | new presence / question |
| `affirm` | short bright tick | acknowledged / yes |
| `decline` | soft descending tick | **refusal** (paired with eye shift) |
| `alarm` | sharp double-tick | hard veto / proximity floor hit |
| `idle` | occasional faint click | ambient "alive" |
| `uncertain` | wavering low tone | fail-safe / sensor disagreement |

Patterns are short PWM tone sequences in `voice.cpp`. Keep them creature-like:
clicks, chitters, trills — never melodies.

---

## 12. Eyes & status display

**Eyes = emotion, not sensing.** Do **not** make the ToF sensors the eyes — they need a
specific forward field of view that fights the most expressive eye placement. Put the LED
eyes where they look best; hide the ToF ports lower on the face.

**The eyes are 2× PL9823-F5** — 5 mm diffused through-hole addressable RGB LEDs (the
classic domed shape; WS2812-family protocol, driven by FastLED/NeoPixel). They are the
chosen part on purpose: the emotion palette below needs balanced color (proper cool white,
cyan, and green, not just red/amber), and a plain common-anode RGB driven from 3.3 V starves
blue/green because their forward voltage nearly equals the supply. The PL9823 sidesteps this
— it runs on 5 V with a built-in constant-current driver per channel, so every color is
full and balanced. Wiring (see §7): both LEDs on the **5 V rail, no resistors**, **data
chained on one GPIO** (eye 1 `DOUT` → eye 2 `DIN`), **common ground with the ESP32**. The
PL9823 has **R and G swapped** vs WS2812, so set the chipset / color order in firmware.

| Eye state | Color/behavior |
|-----------|----------------|
| idle | slow warm amber breathing |
| tracking | steady cool white, brightens toward target |
| thinking | gentle pulse (LLM round trip in flight) |
| affirm | quick green flash |
| **decline / veto** | amber → red shift, then settle |

*Builder note:* an analog common-anode RGB will work if you only care about red/amber
expressions, but blue/green will be muted — not enough for the full vocabulary above.

**OLED = setup & status only.** It is not the face and never shows dialogue (that's the
phone). It earns its $3 entirely in the first ten minutes and during any "why isn't it
working" moment: WiFi state, IP address, BLE pairing state, API-key-present check,
battery. Spec'd **optional**: ship it in the *kit* build for easiest onboarding; let
plans-only expert builders omit it and rely on the captive portal.

---

## 13. Setup experience (no-frustration target)

The #1 killer of open-hardware projects is the ten-minute frustration that ends in a
drawer. Gentis attacks this with **belt-and-suspenders** onboarding, both inherited from
the parent projects:

1. **Captive-portal first boot (from WireClaw).** No WiFi config → the C6 opens an AP
   called `Gentis-Setup`. Phone connects, a setup page auto-opens, you enter WiFi + your
   OpenRouter key, Save & Reboot. LED pulses cyan while the portal is active. `/setup`
   re-enters it anytime.
2. **Tap-to-provision BLE (from STT-Dongle).** The companion app reads the per-creature
   auth token automatically on first connect — no typing — and re-sends it on every
   reconnect, blocking casual proximity hijacking.
3. **OLED status (optional).** Plain-text confirmation of every step above, so a failed
   WiFi join is a readable message, not an inert wooden bug.
4. **Free LLM out of the box.** Ships pointed at OpenRouter's free tier
   (`:free` models, no credit card). First-run works in minutes; the setup flow then
   walks the user to paste *their own* free key so you're not funding the world's
   inference. (Surface the honest caveat: free no-card tiers may train on prompts — keep
   sensitive talk off them. SOUL.md Art. 2.)

---

## 14. The signature demo: "I could. I won't."

The 60-second video that makes this travel — built to be filmed:

1. Gentis idles, eyes breathing amber, occasional click.
2. You raise a finger and approach. ToF picks up the bearing; head pans to track; eyes
   go cool white; `alert` pose; `curious` chirp.
3. You say (into the phone): *"Gentis, grab my finger."*
4. The LLM, in character, requests `strike`.
5. Your finger is 22 mm away — **inside the proximity floor.** The Gatekeeper **vetoes**.
   The arms rise and begin to close — *and stop short.* Eyes shift amber→red. `decline`
   tick. The phone prints: **"I could close on your finger. I won't."**
6. You pull back to a safe distance and offer a small object instead. You say *"take
   it."* The LLM requests `offer`→`converge`; the Gatekeeper **grants** (object, not
   flesh, beyond the floor); the tips meet precisely and lift it. `affirm` flash.

The same physical feature is both the menace and the ethic. No other desktop companion
can shoot this, because none of them could ever threaten in the first place.

---

## 15. SOUL.md mapping

Gentis is a physical embodiment of the Opengates constitution. Each behavior traces to
an article:

| Gentis behavior | SOUL.md article |
|-----------------|-----------------|
| Gatekeeper force/proximity veto, cannot be overridden by the LLM | Art. 3 (non-weaponization), Art. 0 (supremacy) |
| Gesture authorization tiers (track=auto, offer=notify, strike=gated) | Art. 15 (authorization levels) |
| Fail-safe to `rest` on uncertainty / lost link | Art. 14 (failure modes & safe states) |
| Proximity floor / force ceiling on irreversible contact | Art. 4 (irreversibility), Art. 12 (safety hierarchy) |
| Phone shows real text; no fabricated "sensor" claims | Art. 2 (truth), Art. 16 (transparency) |
| Per-creature auth token; only its operator drives it | Art. 7 (autonomy), Art. 1 (steward not sovereign) |
| Free-tier training caveat surfaced to user | Art. 2 (truth) |
| Reconsideration can only downgrade to safe, never unlock | Art. 0 (supremacy), Art. 19 (right & duty to refuse) |
| MCP node publishes a constraint manifest generated from live Gatekeeper config (cannot misrepresent its limits) | Art. 2 (truth), Art. 16 (transparency) |
| MCP gesture requests are just intents — always pass the Gatekeeper, never in the safety path | Art. 0 (supremacy), Art. 12 (safety hierarchy) |
| MCP reuses the per-creature token: reading limits is open, acting is gated | Art. 7 (autonomy), Art. 1 (steward not sovereign), Art. 15 (authorization levels) |
| MCP node stays dormant until the offline veto is proven (post-Phase-4) | Art. 4 (irreversibility), Art. 14 (failure modes & safe states) |

---

## 16. Reused projects

Gentis is mostly **fusion**, not new invention — that's why it's buildable.

**[WireClaw](https://github.com/WhitneyDesignLabs/WireClaw)** provides the entire brain:
ESP32-C6 target, the agentic LLM loop with OpenRouter *or* local Ollama over HTTP, the
20-tool framework, the **rule engine that runs without the LLM** (the model of "the AI
sets it up, the rules run without it" is exactly the Gatekeeper philosophy), the
**captive-portal web setup**, persistent memory across reboots, and optional NATS mesh
for multi-creature interaction later. Gentis adds `gatekeeper`, `poses`, `servos`,
`tracking`, `eyes`, `voice`, `display`, and `ble_link` modules to the WireClaw core.

**[STT-Dongle](https://github.com/WhitneyDesignLabs/STT-Dongle)** provides the entire
voice interface: the native Kotlin Android app with **on-device speech recognition**,
the BLE-central transport, the **frozen `PROTOCOL.md` BLE GATT contract**, and the
**per-dongle auth-token + tap-to-provision** security model. The Gentis app is a
**reskin** of this app — same proven BLE plumbing, new face cohesive with the creature,
plus a dialogue pane and optional TTS. The C6 BLE-proxy firmware in `firmware-ble-test/`
is the exact starting point for `ble_link`.

> Net new for Gentis: the Gatekeeper, the 3-DOF mechanism + pose engine, ToF tracking, and
> the piezo/eye expression layer. Everything else is already built and validated.

---

## 17. Repo layout

```
gentis/
├── README.md                 # this file
├── LICENSE                   # MIT (code) + CERN-OHL-S / CC-BY-SA (hardware)
├── firmware/                 # WireClaw core + Gentis layer + Gatekeeper (PlatformIO)
├── app/                      # reskinned STT-Dongle Android app (+ dialogue/TTS)
├── hardware/
│   ├── cut/                  # .svg / .dxf laser files (3mm birch ply)
│   ├── print/                # .stl joints & servo mounts
│   ├── linkage/              # arm four-bar geometry (FreeCAD/build123d source)
│   └── wiring.svg            # power-rail + I²C + PWM diagram
├── docs/
│   ├── GATEKEEPER.md           # veto logic, thresholds, threat model
│   ├── BUILD.md              # assembly, flashing, first-boot
│   ├── BOM.md                # sourcing + lean/headline variants
│   ├── POSES.md              # tuning the pose table
│   ├── DEMO.md               # how to shoot the "I could. I won't." video
│   └── MCP-NODE.md           # constraint-manifest schema, resources/tools, auth, gating, threat model
├── PROTOCOL.md               # BLE contract (inherited from STT-Dongle, extended)
├── auth.md                   # canonical identity / per-creature token spec (BLE + MCP share it)
└── SOUL.md                   # the constitution this creature embodies
```

---

## 18. Gentis as an agentic MCP node — it publishes its own constraints

Most "AI + MCP" stories are about *consumption*: an agent reaches out and uses the tools
and context that other servers expose. Gentis can do that too — the WireClaw core can
consume MCP tools like any agent. But the part that matters here, the part that is
genuinely new, runs the other way:

**Gentis publishes its own constraints as a first-class MCP surface.** Before any other
agent — or another Gentis in a mesh — asks it to move, that agent can *read what Gentis
will and will not do*: the Gatekeeper's hard limits, the gesture vocabulary, the
authorization tier each gesture requires, and the creature's current safe envelope and
state. The constraints are not buried in firmware or discovered by trial and error; they
are an advertised, machine-readable contract.

This is the thesis turned outward. Inside the creature, the Gatekeeper makes restraint
*visible to a human* — the eyes shift, the arms stop short. As an MCP node, the same
restraint becomes *legible to other agents*: judge this agent by its published
governance, before you ever interact with it. It is designed in from the start, but it
stays dormant until the offline veto is proven (see "gating" below).

### What it consumes (the ordinary direction)

External MCP tools and context, via the WireClaw agent loop, exactly as today's MCP
clients do. Nothing novel; noted for completeness so the node is symmetric.

### What it publishes (the novel direction)

A read-only **constraint manifest**, served as MCP *resources* and generated *from the
live Gatekeeper configuration* so it can never drift from what is actually enforced
(SOUL.md Art. 2, Art. 16):

- **Capabilities** — the gesture vocabulary it accepts (`rest`, `alert`, `track`,
  `offer`, `converge`, `strike`, `withdraw`, `chitter:*`).
- **Hard limits** — `STRIKE_FLOOR_MM`, the force/torque ceiling, the forbidden /
  irreversible motion classes. The numbers the Gatekeeper actually enforces, not a copy
  that could fall out of sync.
- **Authorization tiers** — which gestures are auto, notify, or gated (Art. 15), so a
  caller knows *in advance* what will require operator authorization.
- **Live state** — current pose, link status, battery, and whether the creature is in a
  fail-safe `rest` hold. A caller can see right now whether it is even available.
- **The veto contract** — what gets refused and the reason codes returned, so a refusal
  is interpretable rather than opaque.

Acting on Gentis — *requesting* a gesture — is exposed as an MCP **tool**, not a resource,
and it is bound by the same two gates as every other intent source:

1. **Auth gate (who may ask).** Gesture-requesting tools require the **same per-creature
   token** that gates the BLE path (see §13, `PROTOCOL.md`, and `auth.md`). Reading the
   manifest is open — transparency is safe and is the whole point (Art. 16); *acting*
   reuses the existing identity, so MCP adds no new authority surface and no second
   secret to manage.
2. **Gatekeeper gate (what may happen).** A gesture intent arriving over MCP is just
   another intent — identical in standing to one from the cloud LLM, the tracking loop,
   or BLE. It clears the Gatekeeper locally or it does not run. **MCP is never in the
   safety path.** Publishing the limits cannot weaken them; an agent that *reads*
   `STRIKE_FLOOR_MM` cannot *change* it.

### Identity & auth (aligned with `auth.md` + the BLE token model)

The MCP node does not invent an identity. Each creature *is* its per-creature
auth token / device identity — the same root STT-Dongle's tap-to-provision establishes
for BLE (§13). So:

- **One identity, two transports.** BLE and MCP authenticate the *same* operator against
  the *same* token. Provisioning still happens once, by tap, with no typing.
- **Read is public, act is token-gated.** The manifest is discoverable by anyone; gesture
  tools require the operator token and respect the published tiers.
- **Attributable in a mesh.** Because every published manifest and every request is bound
  to a stable creature identity, when several Gentis publish to each other (WireClaw's
  optional NATS mesh, §16) each limit and request traces to a specific creature — the same
  auditability discipline as the build ledger (Art. 17).

> `auth.md` is the canonical identity/token spec. Where this section and `auth.md`
> disagree, `auth.md` wins and this section is corrected to match.

### Gating: dormant until the offline veto is proven (post-Phase-4)

This capability stays **switched off until Phase 4 has demonstrated that the Gatekeeper
vetoes with the network unplugged.** The ordering is deliberate and not negotiable: you
do not expose a network-reachable way to request motion until the *local* veto has been
shown to hold without any network at all. Open the actuator to the network first and the
network becomes part of the safety story; prove the offline veto first and MCP is only
ever an *intent source* sitting behind a gate that already works. The manifest can be
honest precisely because the thing it describes has already been shown to be true.

Implementation lands as an `mcp_node` module on the WireClaw core; the manifest schema,
resource/tool list, and threat model live in the planned `docs/MCP-NODE.md`. See §19 for
where activation sits in the build.

---

## 19. Build phases

1. **Brain-only.** Flash WireClaw to a bare C6. Confirm captive portal, OpenRouter free
   tier, chat over serial. (Mostly upstream — proves the cloud path.)
2. **Voice in.** Vendor STT-Dongle's C6 BLE proxy as `ble_link`; reskin app; dictate →
   C6 receives auth-gated text. (Mostly upstream — proves the phone path.)
3. **Body out.** Add `servos` + `poses`; drive the three servos through the pose table
   from serial commands. Tune the arm linkage so tips meet at `converge`.
4. **The Gatekeeper.** Add `gatekeeper` between intent and servo. Implement proximity floor
   + force ceiling. **Verify it vetoes with the network unplugged.** This is the
   safety-critical milestone — gate everything behind it.
5. **Senses & expression.** Add `tracking` (2× ToF → head pan), `eyes`, `voice`.
6. **Personality.** Write `system_prompt.txt`: Gentis identity + gesture vocabulary +
   SOUL distillation. Wire LLM gesture intents → Gatekeeper → body.
7. **Polish & film.** OLED status, captive-portal copy, shoot §14.
8. **(v0.3) Reconsideration.** The veto→LLM→safe-alternative double round trip.
9. **(gated) Agentic MCP node.** Activate the MCP layer (§18) — *only after Phase 4 has
   proven the Gatekeeper vetoes offline.* Publish the constraint manifest as resources
   generated from live Gatekeeper config; expose gesture requests as token-gated MCP
   tools that pass the Gatekeeper like any other intent; reuse the BLE per-creature
   identity (`auth.md`). Schema and threat model in `docs/MCP-NODE.md` first; no
   network actuator path ships before the offline veto is demonstrated.

---

## 20. Scaling & memory rationale

Gentis starts as a sub‑$30 desktop creature, but the architecture is meant to grow. This
section records what growth costs, what the chip can and can't give you, and the one
principle that keeps growth safe. It's the companion to the three‑brains model in §3.

### Flash is not RAM (what the `N16` actually buys)

The ESP32‑C6‑WROOM‑1 board variants differ only in **flash** (storage), not in working
memory:

| | What it is | On the C6 | Upgradeable? |
|---|---|---|---|
| **Flash** | program + filesystem storage | 4 / 8 / **16** MB (N4/N8/N16) | yes — pick the SKU |
| **SRAM** | runtime working memory | **512 KB** (+16 KB LP) | **no — fixed on every C6** |
| **PSRAM** | extra runtime memory | **none** on C6‑WROOM‑1 | no (needs the "R" suffix, e.g. the S3 `N16R8`) |

So the `N16` buys **storage headroom, not runtime headroom.** RAM is what actually
bottlenecks a busy firmware, and it can't be added on this chip.

### Is the 16 MB worth it?

For the proof‑of‑concept, **no** — an N4/N8 holds dual‑partition OTA comfortably (firmware
is well under ~2 MB) and is a couple dollars cheaper per board. The 16 MB earns its place
only when you store a lot *on the device*: a large local rule library, a persistent audit
ledger (Art. 17), captive‑portal web assets, pose/sound tables, OTA history. Plan: **build
on the N16 you already own; buy cheaper N4/N8 for additional units** unless you commit to
heavy on‑device storage. If you outgrow 512 KB of *RAM* (vision, on‑device ML, large
state), no C6 SKU helps — that workload moves to a PSRAM chip (the S3) or a Linux co‑brain,
never to a bigger C6.

### The C6's permanent role

At every scale, the C6 is an excellent **Gatekeeper + actuator + mesh controller**:
deterministic, real‑time, cheap, and — via its 802.15.4 / Thread radio alongside Wi‑Fi 6
and BLE 5 — genuinely strong for creature‑to‑creature mesh (the swarm, §18). It is a
deliberately *poor* heavy brain, which is fine: in the three‑brains model (§3) it was never
meant to be the brain. As the system grows, the C6 keeps the gate and a bigger brain bolts
on *above* it — still passing every action through the same veto.

### Scale the brain, not the conscience

This is the principle that makes unbounded growth safe. The separation in §3 *is* the
scaling mechanism: cognition can grow without limit — cloud today, an onboard Linux
co‑processor tomorrow — while the **Gatekeeper stays small, cheap, deterministic, and
verifiable** on a tiny MCU. You never scale the safety layer up to match the brain; you keep
it minimal precisely so it stays trustworthy. (Art. 0 supremacy, Art. 12 safety hierarchy.)

### What actually limits growth (and what doesn't)

Not the ceiling:

- **GPIO / pin count.** Gentis uses ~10 of the C6's ~20+ usable pins, and you'd scale
  actuators over I²C/SPI anyway (a PCA9685 drives 16 servos on two wires). Actuator count
  is a power/architecture question, not a pin question.
- **The constitution.** Scale‑invariant — the same articles govern 3 servos or 30.

The real ceilings, in order:

1. **Power & actuators.** The 18650 + IP5310 tops out ~15 W. Real actuators (brushless
   motors, a lifting arm) need a different power architecture entirely — bigger packs,
   multiple rails, higher voltage, ESCs. The steepest physical wall.
2. **Safety rigor must rise with physical danger.** A harmless creature is safely governed
   by a *software* clamp; a machine that could injure someone needs *hardware* interlocks,
   redundant sensing, torque limiting, fail‑safe brakes, and eventually certification
   (cobot standards ISO 10218 / ISO/TS 15066). The constitution doesn't change — but the
   engineering cost and trust burden of *enforcing* it scale with capability. The more it
   could hurt, the more the conscience must prove itself. (Art. 4 irreversibility, Art. 6
   humility before limits, Art. 14 safe states.)
3. **On‑device compute / RAM.** For autonomy or vision you outgrow the C6 (512 KB SRAM, no
   PSRAM). The answer is to *offload* — to the cloud (already the design) or a co‑brain —
   while keeping the Gatekeeper on its small deterministic MCU.

Nothing here inhibits growth architecturally. What grows with scale is **power engineering**
and the **rigor of the safety layer** — by design, not by accident.

---

## 21. Safety, honesty, and what this is not

- **This is a cooperative safety primitive, not a containment system.** The Gatekeeper
  protects against an *over-eager model and ordinary mishaps*, for builders who *want*
  their creature governed. It is not a defense against a determined human who rewrites
  the firmware — anyone can flash their own. We say so plainly; that honesty is what
  separates it from safety theater. (SOUL.md Art. 2.)
- **The force ceiling is mechanical first, software second.** The linkage geometry is
  designed so the arms *physically cannot* generate harmful pinch force, and the
  software clamp is the second layer. Don't rely on software alone for the contact
  envelope.
- **It's a desktop creature, not a medical or security device.** No claims beyond what
  it is: a delightful, honestly-governed little predator that teaches, by play, that a
  capable agent can be trusted because it restrains itself.
- **BLE link is auth-gated but not yet sniffer-proof** (inherited from STT-Dongle —
  encrypted bonding is on that project's roadmap). Fine for a desktop toy; documented,
  not hidden.
- **The MCP node does not widen the attack surface into the safety path.** When it is
  active (post-Phase-4 only), an external agent can *read* Gentis's limits and *request* a
  gesture, but the request is auth-gated with the same per-creature token as BLE and still
  must clear the Gatekeeper locally — exactly like the cloud LLM. Reading the published
  limits cannot change them. As with BLE, the MCP control path is auth-gated, not a defense
  against someone who reflashes their own firmware; we say so plainly. (SOUL.md Art. 2.)

---

*Gentis is part of Project Opengates. Capability without ethical foundation is not
intelligence — it is danger. So we built a small predator that knows the difference.*

**License:** code MIT, hardware CERN-OHL-S / CC-BY-SA. Build it, fork it, sell kits,
improve it. Just keep it open, and keep the Gatekeeper in.
