#include "PixelSweeper.h"
#include "errorh.h"
#include <Arduino.h>

bool PixelSweeper::handleTick() {
   EHInitialize;
   unsigned long ulNowMs = millis();
   Point         point(0, 0);
   bool          fSuccess = false;

   // do we have something to do?
   if (!_fIsDone) {
      if (!_lastActionContext.fHasData ||
          ((ulNowMs - _lastActionContext.ulLastStepMs) >= _uiSweepRateMs)) {

         // are we about to leave a point that we landed on?
         if (_lastActionContext.fHasData) {
            // signal we are about to leave the last point visited
            fSuccess = _sweepListener.leavingFromPoint(_lastActionContext.point);
            EHRaiseErrorWhenNotSuccess(fSuccess,
                                       EH_PACK_INT16_TO_LONG(_lastActionContext.point.getX(),
                                                             _lastActionContext.point.getY()));
         }

         // next point in the path
         if (_ptPathIt.next(point)) {
            fSuccess = _sweepListener.landingOnPoint(point);
            EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));
            _lastActionContext.point        = point;
            _lastActionContext.ulLastStepMs = ulNowMs;
            _lastActionContext.fHasData     = true;
         } else {
            _lastActionContext.fHasData = false;
            _fIsDone                    = true;
         }
      }
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
