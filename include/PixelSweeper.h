#ifndef PIXEL_SWEEPER_H
#define PIXEL_SWEEPER_H
#include "ColorManager.h"
#include "DisplaySurface.h"
#include "PointPath.h"
#include "PointPathIterator.h"

class PixelSweeper {

 private:
   DisplaySurface&     _displaySurface;
   const ColorManager& _colorManager;
   const Point&        _ptOrigin;
   PointPathIterator   _ptPathIt;
   unsigned int        _uiSweepRateMs;
   bool                _fIsDone;
   struct {
      bool          fHasData;
      unsigned long ulLastStepMs;
      Point         point;
      CRGB          crgbColor;
   } _lastActionContext;

 public:
   PixelSweeper(const Point& ptOrigin, const PointPath& ptPath, unsigned int uiSweepRateMs,
                DisplaySurface& displaySurface, const ColorManager& colorManager) :
       _displaySurface(displaySurface),
       _colorManager(colorManager),
       _ptOrigin(ptOrigin),
       _ptPathIt(ptPath),
       _uiSweepRateMs(uiSweepRateMs),
       _fIsDone(false) {
      _lastActionContext.fHasData     = false;
      _lastActionContext.ulLastStepMs = 0;
      _lastActionContext.point.set(0, 0);
      _lastActionContext.crgbColor = CRGB::Black;
   }

   bool handleTick();
   bool getIsDone() const { return _fIsDone; }
};
#endif
