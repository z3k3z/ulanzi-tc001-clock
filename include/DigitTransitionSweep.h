#ifndef DIGIT_TRANSITION_SWEEP_H
#define DIGIT_TRANSITION_SWEEP_H
#include "DisplaySurface.h"
#include "PixelGlyph.h"
#include "PixelSweeper.h"

class DigitTransitionSweep : public PixelSweeper::ISweepListener {
 private:
   Point             _ptOrigin;
   const PixelGlyph* _pToGlyph;
   DisplaySurface&   _displaySurface;

 public:
   DigitTransitionSweep(DisplaySurface& displaySurface, const PixelGlyph& toGlyph,
                        const Point& ptOrigin = Point(0, 0)) :
       _displaySurface(displaySurface) {
      initialize(toGlyph, ptOrigin);
   }

   void initialize(const PixelGlyph& toGlyph, const Point& ptOrigin) {
      _pToGlyph = &toGlyph;
      _ptOrigin = ptOrigin;
   }

   bool leavingFromPoint(const Point& point) override;
   bool landingOnPoint(const Point& point) override;
};

#endif
