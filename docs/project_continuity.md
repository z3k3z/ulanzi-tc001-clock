# project-continuity.md

## Project

Custom firmware / clock application for Ulanzi TC001 (ESP32 + 32x8
WS2812 matrix)

## Strategic Intent

This project is intentionally being developed in a craft-oriented way
similar to the Garmin Venu 3 watch-face project:

-   stepwise architecture growth\
-   explicit ownership boundaries\
-   visible device-first validation\
-   avoidance of premature abstraction\
-   strong continuity discipline to prevent architectural drift after
    chat loss

The goal is not just to produce a working clock, but to preserve clarity
of design decisions over time.

## Platform Choice

Arduino-on-ESP32 via PlatformIO was deliberately chosen because it
minimizes irrelevant complexity while preserving full control of
rendering:

-   direct WS2812 matrix access\
-   mature FastLED library\
-   immediate GPIO / serial control\
-   fast iteration cycle\
-   no imposed application architecture

ESP-IDF avoided; AWTRIX/ESPHome avoided.

## Toolchain / Environment

-   PlatformIO in VS Code\
-   esp32dev\
-   clang-format

## Known Hardware Lessons

### CH340 Driver Conflict

Remove WD drive if COM port fails.

### FastLED Dithering Artifact

Disable dithering:

FastLED.setDither(DISABLE_DITHER);

## Coding Standards

-   f, b, ui prefixes\
-   modern C++\
-   no globals

## Error Handling

EHInitialize / EHRaiseError / End label pattern.

## State Management

Table-driven state machines.

## Core Architecture

### Application

Owns: - DisplaySurface - DigitSlots - ColonSeparator -
SerialTimeSyncProvider

### DisplaySurface

Owns FastLED + ColorManager.

### Digit Providers

5x8 primary, H1 override for leading zero.

## PixelGlyph

Supports override color.

## Sweep

Double-pass erase/reveal.

## DigitSlot

Outer + inner state machines.

## ColonSeparator

-   1px wide\
-   rows 2 & 5\
-   1Hz blink

## Layout

H1:1 H2:9 Colon:16 M1:19 M2:27

## Runtime

-   12-hour\
-   HHMM\
-   15ms sweep

## Serial Time Sync

Protocol: TIME:`<epoch>`{=html}

Uses settimeofday()

## Time Authority

providers push → system time → read time()

## Time Sync Gating

Not implemented yet.

## Power Model

External power primary.

## Sound Design

Exploration phase.

## Planned Features

-   double wipe\
-   startup animation\
-   NTP\
-   BLE\
-   sound system

## Continuity Practice

Use doc to prevent drift.
