#ifndef SIMPLE_SWEEP_H
#define SIMPLE_SWEEP_H

#include "DisplaySurface.h"
#include "PixelSweeper.h"

class SimpleSweep : public PixelSweeper::ISweepListener {
 private:
   Point           _ptOrigin;
   DisplaySurface& _displaySurface;

   struct {
      bool fHasData;
      CRGB crgbColor;
   } _lastPointDataCache;

 public:
   SimpleSweep(DisplaySurface& displaySurface, const Point& ptOrigin = Point(0, 0)) :
       _ptOrigin(ptOrigin), _displaySurface(displaySurface) {
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
