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

The project is not being optimized for the shortest route to a finished clock. It is being developed as a durable embedded rendering architecture with clear ownership and incremental device validation.

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
- avoid premature abstraction, but introduce ownership boundaries when they prevent drift

## State Machine Design Pattern
For stateful components, prefer a table-driven state machine:
- define an enumerated state type
- define an enumerated event type
- hold current state privately
- expose `handleEvent(event)`
- expose one `getCurrentState() const`
- represent transitions as a table with rows for current states and columns for events
- place the transition table in the `.cpp` file, not inline in the header

This pattern consolidates the state transition diagram into a single defined location and avoids state behavior being implied by scattered branch logic.

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
- IDigitProvider reference
- DigitSlot array
- ValueTracker

Responsibilities:
- initialize hardware
- configure display theme through DisplaySurface
- initialize four DigitSlot instances
- obtain current time surrogate
- detect changed digits through ValueTracker
- tell changed DigitSlots to begin transitions
- tick all DigitSlots
- flush DisplaySurface when the display is dirty

Important current direction:
- Application should remain orchestration only
- Application should not own per-digit visual lifecycle details
- Application should not own ColorManager directly

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
Owns rendering boundary above FastLED and now owns the single ColorManager associated with the physical surface.

Responsibilities:
- initialize FastLED
- set pixel color
- get pixel color
- clear buffer
- show buffer
- expose `getColorManager()` for semantic color lookup

Important rule:
- `show()` remains separate from mutation
- lower layers do not flush automatically
- FastLED remains hidden beneath DisplaySurface
- render clients should generally receive `DisplaySurface&` rather than both `DisplaySurface&` and `ColorManager&`

FastLED brightness is initialized from the surface-owned ColorManager.

### ColorManager
Theme-driven semantic color provider owned by DisplaySurface.

Current themes stored tabularly:
- RedLed
- TransitYellowGreen
- AgedPhosphor
- WarmBusMarquee

Provides:
- active color
- inactive color
- transition cursor color
- global brightness

Current settled theme:
- WarmBusMarquee

Long-term goal:
semantic color + brightness coordination.

## Geometry / Traversal Utilities

### Point
- x/y coordinate holder
- operator+
- operator=

### Rectangle
- origin + extent

### PointIterator
Traverses rectangle:
- XMajor
- YMajor

### PointPath
Static ordered point collection.

### PointPathIterator
Traverses arbitrary point arrays.

This supports arbitrary sweep ordering independent of rectangle traversal.

## PixelGlyph
Current digit renderer:
- bit-row encoded glyphs
- active/inactive pixels resolved through DisplaySurface's ColorManager
- traversal abstraction separated from glyph semantics

Provides:
- `draw(DisplaySurface&, ...)`
- `drawPixelForPoint(DisplaySurface&, ...)`
- `getPixelColorForPoint(DisplaySurface&, ...)`

Important ownership rule:
- PixelGlyph does not own color policy
- PixelGlyph determines whether a glyph pixel is active or inactive
- DisplaySurface supplies the ColorManager used to convert active/inactive semantics into CRGB values

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

## DigitSlot Architecture
DigitSlot is now the owner of one digit's visual lifecycle.

Responsibilities:
- hold digit origin
- hold current digit value
- hold target digit value
- own DigitTransitionSweep
- own PixelSweeper
- initialize its displayed digit
- begin a transition to a new digit value
- advance transition work during `handleTick()`
- expose current state through `getCurrentState() const`

DigitSlot prevents Application from becoming the owner of transition mechanics.

Current DigitSlot state model:
- Idle
- Transitioning
- Complete
- Count sentinel

Current DigitSlot event model:
- BeginTransition
- SweepComplete
- CompleteAcknowledged
- Count sentinel

State transitions are represented in a transition table in `DigitSlot.cpp`.

Design intent:
- ValueTracker remains semantic numeric state
- DigitSlot owns visual digit state
- Application coordinates between them without owning transition internals

Known minor design compromise:
- DigitTransitionSweep currently requires a glyph at construction, so DigitSlot construction may use an initial/fallback glyph. This can be cleaned later by allowing DigitTransitionSweep to be constructed without an active target glyph and initialized before use.

## Sweep Architecture

### PixelSweeper
Traversal engine only.

Responsibilities:
- timing
- path stepping
- listener dispatch
- restartable sweep lifecycle

No rendering policy embedded.

Current important behavior:
- `restart()` resets path iterator, done flag, and last action context
- listener remains a stable reference for the lifetime of the sweeper
- the listener is not expected to change between sweeps
- the extra leave step after the final landed point is semantically important because it restores the final cursor point

### ISweepListener
Virtual interface:
- `landingOnPoint()`
- `leavingFromPoint()`

### SimpleSweep
Direct cursor sweep implementation.

Current behavior:
- caches the color under the cursor when landing
- restores the cached color when leaving
- obtains transition cursor color through DisplaySurface's ColorManager

### DigitTransitionSweep
Current digit transition implementation:
- `landingOnPoint()` draws the transition cursor
- `leavingFromPoint()` restores the corresponding pixel from the target glyph
- obtains transition cursor color through DisplaySurface's ColorManager
- no longer receives ColorManager separately

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
- temporary clock surrogate
- future HHMM clock logic

Current query model:
- digit offset counted from right

Current implementation direction:
- when an update changes the value, both previous and current values are formatted once into zero-padded strings
- per-digit comparison uses the cached text representation
- digit extraction uses character conversion, e.g. `'5' - '0'`

Future pressure point:
- ValueTracker currently tracks semantic previous/current value
- if visual transitions can overlap new semantic updates, the system may eventually need to distinguish semantic value from displayed/committed visual value
- do not move visual transition state into ValueTracker

## Current Runtime Behavior
Application renders four digits through DigitSlot instances.

Value source currently:
- HMMT
where:
- H = low hour digit
- MM = minute
- T = high digit of seconds / tens of seconds

This is intentionally sufficient for visible transition testing.

Current behavior:
- Application obtains HMMT value
- ValueTracker identifies changed digit offsets
- Application calls `beginTransitionTo()` on changed DigitSlots
- Application ticks all DigitSlots
- DisplaySurface is shown when display work occurred
- digit transitions are functioning on device

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

Because ColorManager now belongs to DisplaySurface, future brightness/theme coordination should be implemented at or below the DisplaySurface level rather than in Application.

## Current Architecture Checkpoint
The project has now crossed from a procedural render loop into slot-owned digit rendering:
- Application orchestrates
- DisplaySurface owns hardware boundary and color policy
- PixelGlyph owns glyph semantics
- DigitSlot owns digit visual lifecycle
- PixelSweeper owns traversal timing
- DigitTransitionSweep owns transition pixel behavior
- ValueTracker owns semantic numeric change detection

This is a major architecture checkpoint and should be treated as durable.

## Immediate Next Direction
The most natural next directions are now:

### Carry / rollover choreography
Introduce right-to-left sequencing so that digit transitions can behave more like mechanical or transport-style rollovers.

Potential question:
- should carry sequencing be owned by Application, a future coordinator, or a higher-level multi-digit display object?

Avoid letting Application accumulate too much choreography logic.

### Digit semantic identity
Digits may eventually need roles such as:
- HourLow
- MinuteHigh
- MinuteLow
- SecondsTens / temporary test slot

This could support cleaner carry logic and future clock formatting.

### DigitSlot constructor cleanup
Remove the need for a constructor-time placeholder glyph by making DigitTransitionSweep default-initializable or safely uninitialized until `initialize()` is called.

### Sweep profile ownership
The current random 5x7 path still lives near Application. Eventually this path likely belongs in:
- a sweep profile
- a transition policy object
- or a reusable visual behavior definition

Do not move it prematurely unless it starts cluttering Application or multiple transition styles emerge.

## Future Architecture Candidates

### Tick Listener System
Likely needed later:
- independently registered animation actors
- fades
- digit transitions
- timed visual effects

DigitSlot currently reduces immediate need for a generalized tick listener system, but the concept remains likely if multiple independent visual actors emerge.

### MultiDigitDisplay / ClockDisplay Coordinator
Possible future layer above DigitSlot:
- owns four or more DigitSlots
- understands digit roles
- coordinates carry/ripple timing
- shields Application from multi-digit choreography

This may become preferable if Application begins to contain sequencing policy.

### Sound
Possible future ambient sweep sound:
- speaker confirmed usable
- likely sparse event-based usage only

Avoid coupling sound directly into PixelSweeper. Prefer an event/listener or higher-level visual/audio coordinator if sound is revisited.

### Seasonal Background Themes
Still parked:
- July 4th
- Christmas twinkle

May later be rejected if visually noisy.

If implemented, background effects should respect DisplaySurface/ColorManager ownership and avoid interfering with digit readability.
