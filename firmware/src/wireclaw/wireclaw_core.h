/*
 * wireclaw_core.h — Gentis-added shim header for the vendored WireClaw core.
 *
 * This is NOT part of upstream WireClaw. It exposes the vendored core's entry
 * points so the Gentis firmware entry (src/main.cpp) can own the Arduino
 * setup()/loop() and call into the brain. The matching definitions live in
 * wireclaw/main.cpp, where the upstream setup()/loop() were renamed to
 * wireclaw_setup()/wireclaw_loop() (see wireclaw/VENDOR.md for the exact diff).
 */
#pragma once

/* Bring up the vendored WireClaw core (was WireClaw's setup()). */
void wireclaw_setup();

/* One iteration of the vendored WireClaw core (was WireClaw's loop()). */
void wireclaw_loop();
