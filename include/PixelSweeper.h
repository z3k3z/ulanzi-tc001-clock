#ifndef PIXEL_SWEEPER_H
#define PIXEL_SWEEPER_H
#include "PointPathIterator.h"

class PixelSweeper {
 public:
   class ISweepListener {
    public:
      virtual ~ISweepListener() {}

      virtual bool leavingFromPoint(const Point& point) = 0;
      virtual bool landingOnPoint(const Point& point)   = 0;
   };

 private:
   PointPathIterator _ptPathIt;
   ISweepListener&   _sweepListener;
   unsigned int      _uiSweepRateMs;
   bool              _fIsDone;
   struct {
      bool          fHasData;
      unsigned long ulLastStepMs;
      Point         point;
   } _lastActionContext;

 public:
   PixelSweeper(const PointPath& ptPath, unsigned int uiSweepRateMs,
                ISweepListener& sweepListener) :
       _ptPathIt(ptPath),
       _sweepListener(sweepListener),
       _uiSweepRateMs(uiSweepRateMs),
       _fIsDone(false) {
      _lastActionContext.fHasData     = false;
      _lastActionContext.ulLastStepMs = 0;
      _lastActionContext.point.set(0, 0);
   }

   bool handleTick();
   bool getIsDone() const { return _fIsDone; }
};
#endif
