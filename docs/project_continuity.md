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
- epilogue via `End:` label

Pattern:
- collect file / line / packed context
- branch to epilogue
- caller decides severity

This is intentionally lightweight and uniform to reduce visual cyclomatic complexity.

## Current Core Architecture

### Application
Owns top-level composition:
- CoordinateMapper
- LEDBuffer
- DisplaySurface
- PixelGlyph digit assets
- ColorManager
- active PixelSweeper lifecycle

Responsibilities:
- initialize hardware
- render four themed zero glyphs
- stage sequential sweep experiments

### CoordinateMapper
Maps Cartesian `(x,y)` into serpentine WS2812 index:
- 32x8 matrix
- alternating row direction
- bounds checking via status return

### LEDBuffer
Encapsulates LED storage:
- owns CRGB array
- exposes buffer + count

FastLED initialization consumes LEDBuffer storage.

### DisplaySurface
Owns rendering boundary above FastLED.
Responsibilities:
- initialize FastLED
- set pixel color
- get pixel color
- clear buffer
- show buffer

Important rule:
- `show()` remains separate from mutation
- lower layers do not flush automatically

This preserves future composability.

### Point / Rectangle / PointIterator
Minimal geometry layer introduced.

#### Point
- x/y coordinate holder

#### Rectangle
- origin + extent
- containment semantics

#### PointIterator
Traverses rectangle:
- XMajor
- YMajor

Used by:
- PixelGlyph
- PixelSweeper

## PixelGlyph
Current digit renderer:
- bit-row encoded glyphs
- inactive pixel field visible
- active/inactive colors via ColorManager
- traversal via PointIterator

Current glyphs:
- digit 0
- digit 1

Glyph draw now accepts Point origin overload.

## ColorManager
Theme-driven semantic color provider.

Current themes stored tabularly:
- RedLed
- TransitYellowGreen
- AgedPhosphor
- WarmBusMarquee

Provides:
- active color
- inactive color
- transition cursor color

Long-term goal:
semantic color + brightness coordination.

## Brightness / Theme Direction
Current observed tuning:
- global brightness = 50
- inactive field brightness around 8 gives useful vintage lattice visibility

Long-term architectural goal:
ColorManager should eventually coordinate:
- active color
- inactive color
- global brightness
- ambient brightness adaptation

## PixelSweeper
First animation primitive now working on device.

Responsibilities:
- rectangle traversal using PointIterator
- timed cursor movement via millis()
- restore previous pixel color
- draw transition cursor
- completion state

Current behavior:
- one bright cursor sweeps rectangle
- previous pixel restored after advance
- caller flushes via DisplaySurface.show()

Important architectural rule:
PixelSweeper mutates only.
Application decides frame flush.

## Current Application Runtime Behavior
- four themed zero glyphs rendered at startup
- DigitDescriptor array defines:
  - Point origin
  - ColorTheme
- active PixelSweeper allocated per digit
- sweeps digits sequentially
- sweeper destroyed on completion
- next digit staged cyclically

## DigitDescriptor
Current data-driven rendering descriptor:
- Point origin
- ColorTheme

Used to render four zeroes cleanly without hardcoded repeated draw logic.

## Heap Usage Note
PixelSweeper currently uses heap allocation (`new/delete`) inside Application runtime.
This is acceptable for current experimentation but should be monitored.
Potential future refinement:
- reusable member-owned sweeper instance
- resettable actor lifecycle

## Future Architecture Candidates

### Tick Listener System
Likely needed later.
Purpose:
- independently registered animation actors
- staggered digit transitions
- per-pixel fades

### Sound
Possible future ambient sweep sound.
Device speaker confirmed usable.
Likely only event-based:
- minute rollover
- visible transition events
- avoid continuous fatigue

### Seasonal Background Themes
Future experimental option:
- July 4th styling
- Christmas twinkle colors
Potentially removable if visually noisy.

## Immediate Next Focus
Cursor sweep experimentation:
- refine transit-style motion feel
- likely descending row wipe / cursor variants
- explore timing character
- preserve vintage transit display inspiration

Transition architecture remains intentionally experimental.
