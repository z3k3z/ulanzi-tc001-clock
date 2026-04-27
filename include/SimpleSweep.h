#ifndef SIMPLE_SWEEP_H
#define SIMPLE_SWEEP_H

#include "ColorManager.h"
#include "DisplaySurface.h"
#include "PixelSweeper.h"

class SimpleSweep : public PixelSweeper::ISweepListener {
 private:
   Point               _ptOrigin;
   DisplaySurface&     _displaySurface;
   const ColorManager& _colorManager;

   struct {
      bool fHasData;
      CRGB crgbColor;
   } _lastPointDataCache;

 public:
   SimpleSweep(DisplaySurface& displaySurface, const ColorManager& colorManager,
               const Point& ptOrigin = Point(0, 0)) :
       _ptOrigin(ptOrigin), _displaySurface(displaySurface), _colorManager(colorManager) {
      initialize(ptOrigin);
   }

   void initialize(const Point& ptOrigin) {
      _ptOrigin                     = ptOrigin;
      _lastPointDataCache.fHasData  = false;
      _lastPointDataCache.crgbColor = CRGB::Black;
   }
   bool leavingFromPoint(const Point& point) override;
   bool landingOnPoint(const Point& point) override;
};

#endif
