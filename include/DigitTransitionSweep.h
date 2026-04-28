#ifndef DIGIT_TRANSITION_SWEEP_H
#define DIGIT_TRANSITION_SWEEP_H
#include "ColorManager.h"
#include "DisplaySurface.h"
#include "PixelGlyph.h"
#include "PixelSweeper.h"

class DigitTransitionSweep : public PixelSweeper::ISweepListener {
 private:
   Point               _ptOrigin;
   const PixelGlyph*   _pToGlyph;
   DisplaySurface&     _displaySurface;
   const ColorManager* _pColorManager;

 public:
   DigitTransitionSweep(DisplaySurface& displaySurface, const ColorManager& colorManager,
                        const PixelGlyph& toGlyph, const Point& ptOrigin = Point(0, 0)) :
       _displaySurface(displaySurface) {
      initialize(toGlyph, ptOrigin, colorManager);
   }

   void initialize(const PixelGlyph& toGlyph, const Point& ptOrigin,
                   const ColorManager& colorManager) {
      _pToGlyph      = &toGlyph;
      _ptOrigin      = ptOrigin;
      _pColorManager = &colorManager;
   }

   bool leavingFromPoint(const Point& point) override;
   bool landingOnPoint(const Point& point) override;
};

#endif
