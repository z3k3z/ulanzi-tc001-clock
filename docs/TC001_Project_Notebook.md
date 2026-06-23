# TC001 Project Notebook

## Executive Summary

Project: Custom firmware clock for the Ulanzi TC001 (ESP32 + 32x8 WS2812 matrix)

This project is intentionally craft-oriented. The goal is not maximum features but maximum character. The clock should feel like a small piece of electronic machinery rather than a software appliance.

Primary design priorities:

1. Character
2. Simplicity
3. Local ownership
4. Physical-device validation
5. Architectural clarity

---

## Current Status

Status: Stable

Validated on physical hardware:

- 5x8 digit rendering
- Randomized pixel erase/reveal transitions
- Colon separator
- 12-hour display
- Serial time synchronization
- BLE time synchronization
- Web Bluetooth synchronization page
- Timezone handling
- BLE reconnect lifecycle
- Time sync gating

---

## Development Environment

Platform:
- PlatformIO
- VS Code
- esp32dev

Libraries:
- FastLED
- NimBLE-Arduino

Build:
- PlatformIO Project Tasks → Build

Upload:
- PlatformIO Project Tasks → Upload

Monitor:
- PlatformIO Project Tasks → Monitor

Known issue:
- CH340 enumeration can fail when WD Passport drive attached.
- Remove WD drive before debugging serial connection.

---

## Hardware

Device:
- Ulanzi TC001
- ESP32
- 32x8 WS2812 matrix

Power model:
- External USB power is primary operating mode.
- Battery operation is not considered a major design target.

Observed battery life is insufficient for practical standalone clock operation.

---

## Project Personality

This section is more important than many implementation details.

The clock should feel like:

- electronic machinery
- slightly imperfect
- locally owned
- understandable

The clock should not feel like:

- a cloud appliance
- a mobile app accessory
- an IoT product

Transitions should imply mechanism.

Character is frequently preferred over technical perfection.

---

## Architecture

Application owns:

- DisplaySurface
- DigitSlots[4]
- ColonSeparator
- ValueTracker
- SerialTimeSyncProvider
- BleTimeSyncProvider

DisplaySurface owns:

- FastLED integration
- ColorManager

CoordinateMapper performs:

- world-to-display mapping

Application.cpp is the architectural center of the system.

Runtime sequencing should be preserved unless there is a compelling reason otherwise.

---

## Time System

Single authority:

    system time

Model:

    provider
        ->
    settimeofday()
        ->
    system clock
        ->
    Application reads time()

Never maintain duplicate clock state.

Never allow display state to become time authority.

---

## Time Providers

Implemented:

### SerialTimeSyncProvider

Protocol:

    TIME:<epoch>

Example:

    TIME:1777651200

Validated.

### BleTimeSyncProvider

Library:

    NimBLE-Arduino

Protocol:

    TIME:<epoch>

Intentionally identical to serial transport.

Design rule:

    same protocol
    different transports

Validated.

---

## Web Bluetooth Sync

Purpose:

Allow browser to donate current time to the clock without requiring:

- app installation
- cloud service
- account creation

Workflow:

Browser
    ->
Connect
    ->
Write TIME:<epoch>
    ->
Disconnect

Validated using Chromium-based browsers.

---

## Time Sync Gating

Original concern:

Time could change while digit transitions were active.

Current implementation:

Providers are interrogated only when display activity is quiet.

Current implementation uses display activity detection rather than a dedicated queued sync manager.

This approach was selected because it solved the actual problem with minimal architecture growth.

Validated.

---

## Timezone Handling

System time remains UTC.

Display time is local.

Timezone configured via:

    setenv(...)
    tzset()

This was required because BLE sync initially appeared to set GMT time despite receiving correct epoch values.

Root cause was timezone configuration, not BLE transport.

---

## Display System

Display:

    HH:MM

Layout:

    H1 = x27
    H2 = x19
    Colon = x16
    M1 = x9
    M2 = x1

Current choice:

5x8 glyphs

Reason:

Retro appearance preferred over geometric perfection.

Validated through extended use.

---

## Colon Separator

Current implementation:

- one pixel wide
- full glyph height
- two active dots
- inactive pixels rendered with inactive color
- blink without animation

Status:

Acceptable.

Future refinements possible.

Design discussions explored subtle transition effects rather than dominant animation.

---

## Color System

Theme:

    WarmBusMarquee

FastLED dithering:

    DISABLED

Reason:

Inactive pixels exhibited hue instability between refreshes.

Required:

    FastLED.setDither(DISABLE_DITHER);

Validated.

---

## Digit Transitions

Current transition:

Randomized pixel sweep.

Point path:

    PointPath5x8Random

Two phase:

1. Erase
2. Reveal

Implemented via:

    DigitTransitionSweep

Modes:

- Erase
- Reveal
- EraseAndReveal

Validated.

This transition became a major element of project identity.

---

## State Machines

Project standard:

    TableDrivenStateMachine<TState,TEvent>

Reason:

- less boilerplate
- easier inspection
- EH instrumentation retained
- clearer transition ownership

Reference implementation:

    DigitSlotTransitionStateMachine

Future state machines should follow this pattern by default.

---

## DigitSlot

DigitSlot owns:

- current glyph
- transition state
- sweep execution

State:

- Stable
- Transitioning

Execution delegated to:

    DigitSlotTransitionStateMachine

---

## Hardware Lessons

### FastLED Dithering

Disable dithering.

Observed symptom:

Inactive pixels drifted between hue variants.

### Visual Residue

Rare partially transitioned pixels observed.

Not currently considered a defect.

May contribute to perceived physicality of the display.

Observation only.

---

## BLE Lessons

Advertising visibility required explicit advertising payload configuration.

Advertising must restart after disconnect.

Browser disconnect after sync produced the cleanest user experience.

---

## Sound System

Status:

Concept phase only.

No implementation.

Direction:

Celebrate limitations of piezo hardware.

Avoid:

- realistic chimes
- synthetic orchestration

Prefer:

- simple tones
- electromechanical character
- subtle notifications

---

## Rejected Approaches

### Mobile App

Rejected.

Reason:

Maintenance burden exceeded value.

### WiFi-first Time Sync

Deferred.

Reason:

BLE solved the actual problem with less complexity.

### Duplicate Time State

Rejected.

System clock remains sole authority.

### Complex Sync Queue Manager

Deferred.

Display-quiet gating solved the practical issue.

---

## Future Backlog

Potential future work:

- startup animation
- double wipe transitions
- BLE sync mode instead of always-on advertising
- periodic WiFi NTP synchronization
- piezo sound system
- display personality refinements

Backlog intentionally unordered.

---

## Recovery Checklist

If resuming project after long absence:

1. Build
2. Upload
3. Verify serial monitor
4. Verify clock display
5. Verify serial sync
6. Verify BLE advertising
7. Verify browser sync

Only after all pass should new development begin.

---

## Resume Prompt

If all prior chat history is lost:

Read this notebook first.

Key facts:

- Application.cpp is architectural center.
- System time is sole authority.
- BLE sync is complete and validated.
- Character is preferred over technical perfection.
- Physical-device validation overrides reasoning.
- TableDrivenStateMachine is the project state-machine standard.
- 5x8 glyphs are an intentional design choice.
