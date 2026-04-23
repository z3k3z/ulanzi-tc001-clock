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

## Immediate Next Architectural Layer
Next layer should be:

# DisplaySurface

Responsibilities:
- own LED buffer writes
- accept logical `(x,y)` coordinates
- internally call CoordinateMapper
- isolate FastLED index usage

Goal:
No future rendering code should manually think in strip indices.

Desired semantic API shape:
```cpp
setPixel(x, y, color)
```

---

## Planned Layer Growth Order
1. CoordinateMapper ✅
2. DisplaySurface
3. Canvas / placement abstraction
4. Digit renderer
5. Transition / animation engine
6. Clock semantics

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

