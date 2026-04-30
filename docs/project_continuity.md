# project-continuity.md

## Project
Custom firmware / clock application for Ulanzi TC001 (ESP32 + 32x8 WS2812 matrix)

## Strategic Intent
This project is intentionally being developed in a craft-oriented way similar to the Garmin Venu 3 watch-face project:
- stepwise architecture growth
- explicit ownership boundaries
- visible device-first validation
- avoidance of premature abstraction
- strong continuity discipline to prevent architectural drift after chat loss

## Platform Choice
Arduino-on-ESP32 via PlatformIO was deliberately chosen because it minimizes irrelevant complexity while preserving full control of rendering:
- direct WS2812 matrix access
- mature FastLED library
- immediate GPIO / serial control
- fast iteration cycle
- no imposed application architecture

ESP-IDF was intentionally avoided at this stage because it adds low-level complexity too early.
AWTRIX / ESPHome were avoided because they constrain renderer ownership.

## Toolchain / Environment
- PlatformIO in VS Code
- Board target: `esp32dev`
- Formatter: clang-format
- Brace style: no break before braces
- Trailing whitespace cleanup enabled
- C/C++ formatter active on save

## Known Hardware Lesson
CH340 driver may fail to initialize if a Western Digital USB Passport drive is attached simultaneously.
Resolution:
- remove WD drive
- reconnect TC001
- COM port appears correctly in Device Manager

## Coding Standards
- explicit type prefixes:
  - `f` boolean flags
  - `b` bitfield values
  - `ui` unsigned integers
- modern ANSI C++ preferred when clarity improves
- range-based `for` preferred for collections when index is not semantically required
- retain C compatibility where practical in low-level utilities
- no unbound globals when ownership can be expressed cleanly

## Error Handling Architecture
Adopted project-wide:
- status-return + output parameters
- no exceptions
- no sentinel return values

`errorh.h` provides:
- `EHInitialize`
- `EHRaiseError...`
- `EHEmitMsg`
- `EHEmitMsgDebug`
- optional additional context pointer
- epilogue via `End:` label

Pattern:
- collect file / line / packed context
- branch to epilogue
- caller decides severity

## State Management Pattern (Go-Forward Rule)
Stateful components should preferentially use table-driven state machines:
- enumerated states
- enumerated events
- private current state member
- single `handleEvent(event)` entry point
- single `getCurrentState()` query
- one transition table as authoritative STD representation

Transition tables should be declared in headers and defined in `.cpp` files.

Purpose:
- consolidate STD into one defined location
- reduce distributed conditional logic
- simplify audit and testing

## Current Core Architecture

### Application
Owns top-level composition:
- CoordinateMapper
- LEDBuffer
- DisplaySurface
- IDigitProvider reference
- 4 DigitSlot instances
- ValueTracker

Responsibilities:
- initialize hardware
- initialize slots
- obtain current time surrogate
- detect changed digits
- dispatch slot transitions
- flush display only when dirty

### DisplaySurface
Owns rendering boundary above FastLED.

Responsibilities:
- initialize FastLED
- set pixel color
- get pixel color
- clear buffer
- show buffer
- own ColorManager

Important ownership rule:
- ColorManager now belongs exclusively to DisplaySurface
- render clients obtain semantic colors through DisplaySurface

### ColorManager
Theme-driven semantic color provider owned by DisplaySurface.

Provides:
- active color
- inactive color
- transition cursor color

Current settled theme:
- WarmBusMarquee

Long-term goal:
semantic color + brightness coordination.

## Digit Architecture

### IDigitProvider
Abstract interface:
- supplies glyph by numeric digit

### FiveBySevenDigitProvider
Previous known-good glyph source.
- width = 5
- height = 7
- retained as a fallback / comparison provider rather than deleted

### FiveByEightDigitProvider
Current active digit provider:
- width = 5
- height = 8
- uses the full vertical height of the 32x8 display
- deployed and visually verified on device
- fits the intended retro / bus-marquee aesthetic better than initially expected

The prior FiveBySevenDigitProvider should remain available as a fallback / comparison provider.

Current sweep path for digit transitions is now 5x8:
- 40 points
- includes row 7
- replaces the earlier 5x7 / 35-point transition path for the active provider

Design implication:
- future transition work should assume full-height glyphs unless explicitly testing an alternate provider
- avoid hardening provider-specific geometry into unrelated classes

## PixelGlyph
Current digit renderer:
- bit-row encoded glyphs
- active/inactive pixels via DisplaySurface-owned ColorManager

Provides:
- `draw()`
- `drawPixelForPoint()`
- `getPixelColorForPoint()`

## Sweep Architecture

### PixelSweeper
Traversal engine only.

Responsibilities:
- timing
- path stepping
- listener dispatch
- restartable traversal

Listener identity is fixed for sweeper lifetime.

### DigitTransitionSweep
Current active digit transition:
- leaves old glyph
- lands new glyph

### Planned Investigation: Double-Wipe Transition
Desired alternate transition:
- vertical or horizontal band cursor
- outbound pass erases current digit
- return pass reveals target digit

Architectural implication:
- future transition ownership should avoid hard-coding one sweep model too deeply into DigitSlot
- likely future transition behavior boundary if multiple styles stabilize

## DigitSlot
DigitSlot now owns per-digit behavior:
- current value
- target value
- render origin
- DigitTransitionSweep
- PixelSweeper
- internal state machine

Current states:
- Idle
- Transitioning
- Complete

State transitions use table-driven STD.

DigitSlot is now the correct locus for future per-digit behavior expansion.

### Future Slot Position Consideration
Startup animation introduces likely need for:
- home origin
- current animated origin

Meaning current origin should be thought of as render position, not permanent identity.

## Planned Startup Animation
Desired behavior:
- on startup, digit slots slide into place one at a time
- entry may come from left or right

Architectural implication:
- slot origin likely becomes mutable during startup sequence

## ValueTracker
Numeric state tracker.

Responsibilities:
- hold previous/current values
- cache zero-padded text representations
- answer per-digit changes

Current digit width:
- 4 digits

## Current Runtime Behavior
Current visible time surrogate:
- HMMT
where:
- H = low hour digit
- MM = minute
- T = tens of seconds

Used intentionally for visible transition testing.

Current active display configuration:
- four 5x8 digits
- full-height glyph rendering on the 32x8 matrix
- 5x8 random transition path
- DigitSlot-driven per-digit transition lifecycle

## Planned Time Source Evolution
Current `_getTimeAsInt()` remains temporary.

Future requirement:
- local network time sync
- minimal user setup
- credential sensitivity awareness

Likely future subsystems:
- TimeProvider
- NetworkManager
- CredentialStore

Possible mechanisms:
- Wi-Fi credential persistence in ESP32 NVS/preferences
- NTP/SNTP sync
- graceful offline fallback

## Design Discipline Going Forward
Before adding new helpers or ownership:
ask whether the decision freezes one of the known future directions:
- alternate transitions
- mutable slot position
- larger glyph geometry
- network time source layering

If yes, bias toward neutral ownership and naming.
