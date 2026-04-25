# project-continuity.md

## Project
Ulanzi TC001 custom clock / visual display system on ESP32 using PlatformIO + Arduino framework.

## Core Intent
Build a custom clock that treats the 32x8 LED matrix as a deliberately constrained visual medium, emphasizing character, motion, and low-resolution design rather than merely showing time.

The project should evolve with the same discipline used in the Garmin watch-face project:
- preserve architecture
- avoid premature abstraction
- let hardware observations drive design
- keep rendering ownership explicit

---

# Toolchain / Environment

## Chosen stack
- PlatformIO
- Arduino framework on ESP32
- FastLED
- VS Code

## Why Arduino-on-ESP32 was chosen
Chosen because it minimizes irrelevant complexity while preserving full control of rendering:
- immediate WS2812 matrix access
- GPIO simplicity
- mature FastLED support
- fast compile / deploy cycle
- no imposed framework architecture

Avoided:
- ESP-IDF (premature low-level complexity)
- AWTRIX / ESPHome (would constrain renderer ownership)

## Hardware lesson learned
CH340 driver may fail to initialize if a Western Digital USB Passport drive is attached.
Removing the drive and reconnecting the TC001 allows Device Manager to create the COM port.

## C++ direction
Project should prefer newer ANSI C++ constructs when they improve clarity:
- `static_cast<>`
- `nullptr`
- `enum class`
- modern initialization where useful

Do not force novelty where clarity declines.

---

# Current Architecture

## Top-level ownership
`Application` is the runtime owner.

`main.cpp` should remain a thin shell only.

## Current ownership model

`Application` owns:
- `CoordinateMapper`
- `ColorManager`
- `LEDBuffer`
- `DisplaySurface`
- glyph instances
- timing/demo state

## Dependency direction

`CoordinateMapper` -> maps logical coordinates to serpentine LED indices
`LEDBuffer` -> owns LED storage
`DisplaySurface` -> owns rendering onto LED buffer and hides FastLED
`ColorManager` -> semantic theme/color source
`PixelGlyph` -> static bitmap glyph drawing
`Application` -> orchestration / experiments / demo behavior

## FastLED ownership
FastLED is intentionally hidden under `DisplaySurface`.

Reason:
`DisplaySurface` owns LED presentation semantics and already depends on `LEDBuffer`.

FastLED should not leak upward unless justified.

## Initialization rule
Hardware initialization should occur explicitly through `DisplaySurface.initialize()` rather than constructor side effects.

Reason:
constructors should establish object validity, not hardware side effects.

---

# Error Handling Architecture

## Adopted pattern
Status-return plus output parameters.

No exceptions.
No sentinel return values for mixed success/data.

## Standard flow
Each function:
- initializes error state with `EHInitialize`
- uses helper macros to detect failure
- macro captures:
  - file
  - line
  - context payload
- macro branches to `End:` label
- epilogue returns success/failure

## Error context object
`ErrorHandler`

Current structure includes:
- boolean error flag (`fInError`)
- file
- line
- context payload

## Macro philosophy
Uniform error-checking should visually fade into the background through consistency.

## Logging philosophy
Low-level layers may suppress emission.
Caller decides severity.

---

# Coding Standard

## Naming prefixes by declared type
Examples:
- `f` = boolean flag
- `b` = bitfield
- `ui` = unsigned int

## C compatibility preference where practical
Noise accepted if utility remains broad.

## Formatting
clang-format enforced.

Preferences:
- no break before braces
- one constructor initializer per line
- glyph bitmap tables protected when needed with `clang-format off/on`

## Constructor style
One semantic initializer per line.

---

# Coordinate System

## Matrix
32 x 8 LED matrix

## Physical reality
Serpentine strip:
- alternating row direction
- row parity changes LED index direction

## Abstraction rule
All upper layers work in Cartesian logical coordinates.

## CoordinateMapper responsibility
Translate logical `(x,y)` to physical LED index.

Bounds checking returns success/failure under project error architecture.

---

# Rendering Layers

## LEDBuffer
Owns:
- CRGB storage
- size

Provides cohesive ownership rather than exposing raw global array.

## DisplaySurface
Responsibilities:
- set logical pixel color
- clear
- show
- FastLED ownership

Future:
may gain primitive drawing if justified.

## PixelGlyph
Current responsibility:
static bitmap glyph rendering only.

Current constructor-owned state:
- row bitmap pointer
- width
- height

No transition logic belongs here yet.

## Current glyph direction
Pixel digits are expected because display resolution is extremely low.

Glyph size still intentionally undecided because transition style may influence final geometry.

---

# Visual Design Direction

## Current proven effect
Dormant lattice / inactive pixels significantly improves perceived vintage character.

Equivalent to inactive segments on Garmin watch face.

## Important discovery
Inactive pixel visibility depends strongly on:

`pixel RGB value × global brightness`

These must be treated as coordinated.

## Current successful baseline
- global brightness: 50
- inactive red: 8

This produced useful visible dormant field.

---

# Color Architecture

## ColorManager introduced
Purpose:
centralize semantic color requests.

Callers request:
- active color
- inactive color
- cursor color
- global brightness

No raw CRGB constants should spread outward.

## Theme model
Current table-driven theme definitions.

Each theme defines:
- active color
- inactive color
- cursor color
- global brightness

## Current themes
- RedLed
- TransitYellowGreen
- AgedPhosphor
- WarmBusMarquee

## Reason for table model
Simple tuning through data edits rather than switch logic.

## Long-term color goal
ColorManager should eventually coordinate:
- semantic requested color
- global brightness
- ambient brightness input

Meaning:
"bright red" becomes semantic, not fixed RGB.

## Future likely direction
Base theme value -> brightness compensation -> emitted CRGB

---

# Future Visual Possibilities (Parked)

## Seasonal background system (parked only)
Possible date-driven themes:
- July 4 red/white/blue
- late December Christmas twinkle

Must remain subordinate to readability.

Possible rejection if visually noisy.

Rule:
clock first, atmosphere second.

---

# Current On-Device State

## Proven working
- two pixel glyph digits render correctly
- dormant field visible
- theme comparisons render on device
- ColorManager integrated and deployed

## Current comparison mode
Four theme variants displayed side-by-side for visual evaluation.

---

# Immediate Next Step

## Transition experiment (not yet implemented)
Planned first transition:

Descending row wipe, left to right.

## Cursor concept
A moving cursor sweeps row-major across digit cell.

Behavior:
- cursor white when landing on target lit pixel
- dormant field when target pixel off
- committed pixels remain behind cursor

## First iteration deliberately simple
No fade yet.

## Timing target
Vintage transit / bus marquee inspiration.

Likely first timing range:
15–20 ms per cursor step.

## Architectural note
Transition logic should not yet be placed in `PixelGlyph`.

Keep first implementation experimental in `Application` until motion vocabulary stabilizes.

---

# Discipline Reminder
Avoid premature abstraction driven by imagined future needs.

Prefer:
- visible device experiments
- hardware-guided decisions
- explicit code until repetition proves abstraction need.
