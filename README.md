# TC001 Custom Clock Firmware

## Project
ESP32-based firmware for the Ulanzi TC001 pixel clock LED display featuring a custom-rendered LED matrix clock with transition animations and extensible architecture.

## Overview
This project implements a custom clock for the TC001 device using:

- ESP32 (Arduino framework via PlatformIO)
- 32×8 WS2812 LED matrix
- FastLED rendering pipeline
- Modular digit rendering and transition system

The goal is full control over rendering and behavior, prioritizing:

- visual clarity
- architectural discipline
- device-first validation
- long-term extensibility

## Features

### Display
- Full-height 5×8 digit glyphs
- Leading-zero suppression via alternate digit provider
- Independent colon separator display actor
- Warm retro “bus marquee” visual theme

### Animation
- Per-digit transitions
- Double-pass erase → reveal pixel sweep
- Randomized 5×8 traversal path
- Non-blocking update loop

### Time System
- 12-hour display format
- Serial-based time synchronization
- System time authority model (`settimeofday()`)

## Hardware

- Device: Ulanzi TC001
- MCU: ESP32 (WROOM)
- Display: 32×8 WS2812B matrix
- USB: CH340 serial bridge

## Developer Environment Setup

### Requirements
- Visual Studio Code
- PlatformIO extension
- Python (for PlatformIO backend)
- CH340 USB driver

### Clone Repository
git clone <your-repo-url>
cd <repo-folder>

### Open in VS Code
File → Open Folder → select repo

### Build
pio run

### Upload
pio run --target upload

### Serial Monitor
pio device monitor

Baud rate: 115200

## Project Structure

/src
  Application.cpp/h
  DisplaySurface.cpp/h
  DigitSlot.cpp/h
  DigitTransitionSweep.cpp/h
  PixelSweeper.cpp/h
  PixelGlyph.cpp/h
  ValueTracker.cpp/h
  ColonSeparator.cpp/h
  SerialTimeSyncProvider.cpp/h
  FiveByEightDigitProvider.cpp/h
  H1FiveByEightDigitProvider.cpp/h

/lib
  errorh.h

## Key Architectural Concepts

### System Time Authority
providers push → system time → application reads

### Digit Rendering
- IDigitProvider abstraction
- 5×8 full-height glyphs
- H1 digit override via alternate provider

### Transition Model
DigitSlot:
- outer lifecycle state machine
- internal transition state machine

Transition flow:
Erase → Reveal (double-pass)

### Display Actors
ColonSeparator demonstrates non-digit rendering actors.

### Rendering Pipeline
Application.tick():
→ update time
→ update digits
→ update actors
→ show() only when dirty

## Serial Time Synchronization

Protocol:
TIME:<unix_epoch>

Behavior:
- parse serial input
- validate
- call settimeofday()
- update system time

## Known Hardware Considerations

### CH340 Driver Conflict
Remove WD USB drives if COM port fails.

### FastLED Dithering
Disable dithering:
FastLED.setDither(DISABLE_DITHER);

## Power Model
- Battery ≈ 10 hours
- External power recommended

## Development Notes

### Time Sync Safety (Pending)
Do not apply time sync during active transitions.

### Sound System (Planned)
- piezo-based tones
- minimal, rhythmic
- non-blocking

## Future Features
- Wi-Fi / NTP sync
- BLE configuration
- startup animation
- alternate transitions
- sound system

## Git Workflow

### Tag Release
git tag -a v0.3.0 -m "field trial baseline"
git push origin v0.3.0

## Philosophy

clarity over cleverness
control over convenience
evolution over premature abstraction

## Status
Device verified
Field trial ready
Architecturally stable

## Continuity
Use project-continuity.md to prevent drift and recover state.
