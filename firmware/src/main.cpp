/*
 * Gentis firmware — entry point.
 *
 * Phase 1 (TC-0002): brain-only. This entry brings up the vendored WireClaw
 * core and nothing else. The Gentis expression/safety layer (gatekeeper, poses,
 * servos, tracking, eyes, voice, display, ble_link) is added by LATER
 * directives — the TODO markers below show where it attaches.
 *
 * Per README §9 the eventual loop order is:
 *   gatekeeperTick() -> trackTick() -> ruleTick() -> blePoll() -> aiChat()
 * For now we simply delegate to the WireClaw core's setup()/loop().
 */
#include <Arduino.h>
#include "wireclaw/wireclaw_core.h"

void setup() {
    // TODO(Gentis layer): initialize gatekeeper, servos, tracking, eyes, voice,
    //                     display, ble_link BEFORE / AROUND the brain as needed.
    wireclaw_setup();   // bring up the vendored WireClaw brain (Phase 1: brain-only)
}

void loop() {
    // TODO(Gentis layer): run the reflex/safety ticks every cycle, e.g.
    //   gatekeeperTick(); trackTick(); ruleTick(); blePoll();
    // The Gatekeeper must gate every motion intent BEFORE any servo moves.
    wireclaw_loop();    // one iteration of the vendored WireClaw brain
}
