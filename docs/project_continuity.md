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
This is intentionally lightweight and uniform to reduce visual cyclomatic complexity.
Packed long context commonly stores:
- two 16-bit packed values
- coordinate pairs
- small state payloads

Debug emission is intentionally suppressible by framework flag.

## Current Core Architecture

### Application
Owns top-level composition:
- CoordinateMapper
- LEDBuffer
- DisplaySurface
- ColorManager
- IDigitProvider reference
- ValueTracker

Responsibilities:
- initialize hardware
- render 4 digits
- obtain current time surrogate
- detect changed digits
- render only changed digits

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

FastLED is intentionally hidden beneath DisplaySurface.

### Point / Rectangle / PointIterator / PointPath

#### Point
- x/y coordinate holder
- operator+
- operator=

#### Rectangle
- origin + extent

#### PointIterator
Traverses rectangle:
- XMajor
- YMajor

#### PointPath
Static ordered point collection

#### PointPathIterator
Traverses arbitrary point arrays

This now supports arbitrary sweep ordering independent of rectangle traversal.

## PixelGlyph
Current digit renderer:
- bit-row encoded glyphs
- active/inactive pixels via ColorManager
- traversal abstraction separated from glyph semantics

Provides:
- `draw()`
- `drawPixelForPoint()`
- `getPixelColorForPoint()`

Glyph definitions are no longer owned by Application.

## Digit Provider Architecture

### IDigitProvider
Abstract interface:
- supplies glyph by numeric digit

Contract:
- `bool getDigitFor(unsigned int uiDigit, const PixelGlyph*& pPixelGlyph) const`

### FiveBySevenDigitProvider
Owns digits 0 through 9.

This is now the authoritative glyph source.

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

Current settled theme:
- WarmBusMarquee

Long-term goal:
semantic color + brightness coordination.

## Brightness / Theme Direction
Current observed tuning:
- global brightness increased to improve inactive field visibility
- inactive field kept dim but visible

Long-term architectural goal:
ColorManager should eventually coordinate:
- active color
- inactive color
- global brightness
- ambient brightness adaptation

## Sweep Architecture

### PixelSweeper
Traversal engine only.

Responsibilities:
- timing
- path stepping
- listener dispatch

No rendering policy embedded.

### ISweepListener
Virtual interface:
- landingOnPoint()
- leavingFromPoint()

### SimpleSweep
Current direct sweep implementation.

### DigitTransitionSweep
Current transition implementation:
- leaves old glyph
- lands new glyph

Transition is functioning on device.

## ValueTracker
Numeric state tracker.

Responsibilities:
- hold previous/current values
- cache zero-padded text representations
- answer per-digit changes

Current digit width:
- 4 digits

Used for:
- counter
- temporary clock surrogate
- future HHMM clock logic

Current query model:
- digit offset counted from right

## Current Runtime Behavior
Application renders four digits.

Value source currently:
- HMMT
where:
- H = low hour digit
- MM = minute
- T = tens of seconds

This is intentionally sufficient for visible transition testing.

Only changed digits are re-rendered.

## Immediate Next Direction
Transition architecture should evolve toward per-digit state:

- Idle
- Transitioning
- Complete

This is prerequisite for:
- staggered rollover
- right-to-left carry behavior
- independent digit timing

## Future Architecture Candidates

### Tick Listener System
Likely needed later:
- independently registered animation actors
- fades
- digit transitions
- timed visual effects

### Sound
Possible future ambient sweep sound:
- speaker confirmed usable
- likely sparse event-based usage only

### Seasonal Background Themes
Still parked:
- July 4th
- Christmas twinkle

May later be rejected if visually noisy.
