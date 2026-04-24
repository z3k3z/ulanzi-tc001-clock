# project-continuity.md

## Project Identity
Custom firmware project for the Ulanzi TC001 LED matrix clock using ESP32 hardware, developed in PlatformIO on Windows, targeting full ownership of rendering behavior rather than adapting an existing clock firmware.

---

## Core Architectural Intent
The project is intentionally being built as a clean layered rendering system, similar in discipline to the Garmin watch-face project.

The goal is **full semantic ownership of display behavior**:
- logical coordinate system independent of physical LED strip layout
- future digit rendering and animation built above a stable abstraction stack
- no direct strip-index thinking outside the mapping/surface layer
- explicit low-level control retained

The display is treated as a pixel-addressable matrix, not as a prebuilt clock appliance.

---

## Why Arduino-on-ESP32 via PlatformIO Was Chosen
Arduino-on-ESP32 via PlatformIO was chosen intentionally because it minimizes irrelevant complexity while preserving full control.

Reasons:
- immediate direct access to WS2812 matrix hardware
- mature FastLED library
- fast compile / upload cycle
- direct GPIO control
- direct serial diagnostics
- no imposed architecture
- easy iterative experimentation

Alternatives rejected:

### ESP-IDF
Rejected because:
- adds premature low-level RTOS / driver complexity
- slows early rendering experimentation
- unnecessary for current abstraction goals

### AWTRIX / ESPHome
Rejected because:
- ownership of renderer becomes constrained
- visual system becomes subordinate to framework assumptions

The project intentionally preserves renderer ownership.

---

## Hardware Notes
Device: Ulanzi TC001

Key display characteristics:
- 32 x 8 matrix
- 256 WS2812 LEDs
- physical strip is serpentine wired
- row direction alternates by row parity

Meaning:
- even rows increase left → right
- odd rows increase right → left

Speaker present on board:
- speaker emitted constant tone until buzzer pin configured as input pulldown

Known buzzer suppression:
- configure buzzer pin 15 as INPUT_PULLDOWN

---

## Critical Hardware Lesson Learned
CH340 driver may fail to initialize if a Western Digital USB Passport drive is attached simultaneously.

Observed behavior:
- TC001 inserted
- no COM port appears

Resolution:
- remove WD USB Passport drive
- reconnect TC001
- Device Manager then creates COM port successfully

Treat this as a known Windows environment quirk.

---

## Development Environment
Toolchain:
- PlatformIO in VS Code
- Arduino framework
- ESP32 target

Board used:
- esp32dev

Build:
```bash
pio run
```

Upload:
```bash
pio run -t upload
```

Serial monitor:
```bash
pio device monitor
```

Current test approach:
- manual on-device serial harness preferred over PlatformIO Unity tests for now

Reason:
- embedded Unity runner produced upload-stage failure
- native host testing requires desktop GCC toolchain not currently installed

---

## Coding Standard
### Naming prefixes communicate declared type
Examples:
- `f` = boolean flag
- `b` = bitfield value
- `ui` = unsigned int
- `i` = signed int
- `l` = long
- `psz` = pointer to zero-terminated string

Examples:
- `fInError`
- `uiMappedIndex`
- `iX`
- `pszFile`

### Brace style
Current formatter preference:
- attached braces for control statements

### Formatting enforcement
VS Code formatter active.

Settings include:
- format on save
- trim trailing whitespace
- insert final newline

Formatting sweep was intentionally isolated into its own change set to avoid future diff clutter.

---

## Error Handling Architecture
Project standard is explicit status-return plus output parameters.

Avoid:
- exceptions
- sentinel return values
- magic invalid values

Pattern:
- function returns bool success/failure
- outputs returned through reference parameters

---

## Error Framework (`errorh.h`)
Reusable C-compatible macro-based failure framework established.

### Error context structure
`EHErrorContext`

Fields:
- `bool fInError`
- `const char* pszFile`
- `int iLine`
- `long lData`

### Internal macro-owned local instance
`ehEc`

### Core macros
- `EHInitialize`
- `EHRaiseError(...)`
- `EHRaiseErrorWhen(...)`
- `EHRaiseErrorWhenNotSuccess(...)`
- `EHErrorRaised`
- `EHIsSuccess`
- `EHEmitMsg`

### Flow model
Functions use:
- initialize context
- business logic
- jump to `End:` on failure
- epilogue determines return behavior

Canonical shape:
```cpp
EHInitialize;

... business logic ...

End:
    if (EHErrorRaised) {
        EHEmitMsg;
        return false;
    }

    return true;
```

### Important design rule
Low-level functions may intentionally suppress emission.

Meaning:
- low-level layer raises failure
- caller decides whether logging matters

This is intentional architecture.

---

## CoordinateMapper Layer
First stable semantic layer above raw strip indexing.

### Purpose
Translate logical `(x,y)` into physical LED index.

### Constructor owns matrix dimensions
Fields:
- `_uiWidth`
- `_uiHeight`

### Public method
`bool indexFromCoordinates(int iX, int iY, unsigned int& uiIndex)`

### Coordinate assumptions
- origin `(0,0)` upper-left
- X increases right
- Y increases downward

### Behavior
Bounds validated before unsigned math.

Checks:
- `iX < 0`
- `iY < 0`
- `iX >= width`
- `iY >= height`

### Serpentine mapping
Even row:
- left → right

Odd row:
- right → left

### Error behavior
No emission inside mapper.
Failure intentionally returned upward only.

Reason:
Mapper is low-level and later clipping may make out-of-range requests normal.

---

## On-Device Validation Already Completed
CoordinateMapper has been proven on hardware.

Demo performed:
- one pixel advanced every 250 ms
- logical order left → right, top → bottom
- wraps back to `(0,0)`

This confirmed:
- mapper correctness
- serpentine abstraction correctness
- logical coordinate ownership over physical strip layout

This is considered a major architectural milestone.

---

## DisplaySurface Layer
DisplaySurface is now implemented, integrated into `main.cpp`, built, deployed, and proven on device.

Responsibilities:
- accept logical `(x,y)` coordinates
- call `CoordinateMapper` to obtain a physical LED index
- call `LEDBuffer` to obtain writable LED storage
- set pixel color without exposing strip-index concerns to callers

Current public API:
```cpp
bool setPixelColor(int iX, int iY, const CRGB& color)
```

Design rule:
Future rendering code should not touch raw LED indices directly. It should write through `DisplaySurface` or higher abstractions.

Error behavior:
`DisplaySurface` participates in the `EH` status/epilogue pattern but intentionally does not emit diagnostics automatically. It returns failure upward and lets the caller decide severity.

---

## LEDBuffer Layer
`LEDBuffer` is implemented as the owner of physical LED storage.

Responsibilities:
- own the fixed 256-element `CRGB` array
- expose LED count
- expose indexed LED access through a status-return method
- expose raw buffer access only for the FastLED initialization boundary

Current public concepts:
```cpp
bool getLEDAt(unsigned int uiIndex, CRGB*& pLED)
unsigned int getCount() const
CRGB* getBuffer()
```

`getBuffer()` is tolerated only as a hardware/library integration escape hatch for `FastLED.addLeds(...)`; it should not become a general rendering API.

---

## Current Object Graph
Current `main.cpp` composition:
```cpp
static Application gApplication;
```

`main.cpp` is now a thin Arduino shell:
```cpp
void setup() {
    gApplication.initialize();
}

void loop() {
    gApplication.tick();
}
```

Application now owns:
- `CoordinateMapper`
- `LEDBuffer`
- `DisplaySurface`
- logical sweep state
- timing state

This confirms the intended ownership structure:
- `LEDBuffer` owns storage
- `CoordinateMapper` owns coordinate-to-index mapping
- `DisplaySurface` references both and performs logical pixel mutation
- `Application` owns runtime composition and sequencing

---

## Application Layer
`Application` is now the top-level owner of runtime project state.

Responsibilities:
- device bring-up orchestration
- buzzer suppression setup
- time-step scheduling
- demo sequencing
- calling render operations

Design rule:
`main.cpp` should remain framework shell only.

---

## FastLED Boundary
FastLED is now hidden fully behind `DisplaySurface` during runtime.

`DisplaySurface` now owns:
- FastLED initialization
- frame clear
- frame presentation (`show()`)

This means:
- `Application` no longer directly manipulates FastLED
- raw LED buffer ownership remains in `LEDBuffer`
- FastLED is treated purely as transport infrastructure

Important design decision:
FastLED initialization occurs through a discrete `DisplaySurface::initialize()` method, not in the constructor.

Reason:
embedded object construction should remain lightweight; hardware registration belongs in explicit initialization flow.

Architectural rule going forward:
Higher layers should not call FastLED directly.

---

## Planned Layer Growth Order
1. CoordinateMapper ✅
2. LEDBuffer ✅
3. DisplaySurface ✅
4. Canvas / placement abstraction
5. Digit renderer
6. Transition / animation engine
7. Clock semantics

---

## Rendering Direction
Display strengths should be exploited visually.

Design goal:
- play to LED matrix character
- add interest through digit transitions
- animation sequencing between displayed values

Animation concept intentionally deferred until rendering layers stabilize.

---

## Testing Philosophy
Current preference:
- lightweight manual hardware verification first
- formal test infrastructure later if it stops obstructing progress

Manual serial harness is acceptable during foundational bring-up.

---

## Important Continuity Guardrail
Preserve current layering discipline.

Do not skip directly into digit rendering while bypassing:
- DisplaySurface
- clean ownership boundaries

Architectural drift here would recreate physical-strip concerns in higher layers.

That must be avoided.
