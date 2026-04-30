#ifndef DIGIT_TRANSITION_SWEEP_H
#define DIGIT_TRANSITION_SWEEP_H

#include "DisplaySurface.h"
#include "PixelGlyph.h"
#include "PixelSweeper.h"

enum class DigitTransitionSweepMode {
   Erase = 0,
   Reveal,
   EraseAndReveal,
};

class DigitTransitionSweep : public PixelSweeper::ISweepListener {
 private:
   Point                    _ptOrigin;
   const PixelGlyph*        _pToGlyph;
   DisplaySurface&          _displaySurface;
   DigitTransitionSweepMode _mode;

 public:
   DigitTransitionSweep(DisplaySurface& displaySurface, const PixelGlyph& toGlyph,
                        const Point&             ptOrigin = Point(0, 0),
                        DigitTransitionSweepMode mode = DigitTransitionSweepMode::EraseAndReveal) :
       _displaySurface(displaySurface) {
      initialize(toGlyph, ptOrigin, mode);
   }

   void initialize(const PixelGlyph& toGlyph, const Point& ptOrigin,
                   DigitTransitionSweepMode mode = DigitTransitionSweepMode::EraseAndReveal) {
      _pToGlyph = &toGlyph;
      _ptOrigin = ptOrigin;
      _mode     = mode;
   }

   void setMode(DigitTransitionSweepMode mode) { _mode = mode; }

   bool leavingFromPoint(const Point& point) override;
   bool landingOnPoint(const Point& point) override;
};

#endif
