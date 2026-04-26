#include "PixelSweeper.h"
#include "errorh.h"

bool PixelSweeper::handleTick() {
   EHInitialize;
   unsigned long ulNowMs = millis();
   Point         pathPoint(0, 0);
   bool          fSuccess = false;

   // do we have something to do?
   if (!_fIsDone && (!_lastActionContext.fHasData ||
                     (ulNowMs - _lastActionContext.ulLastStepMs) >= _uiSweepRateMs)) {
      // access the next point in the sweep
      if (_ptPathIt.next(pathPoint)) {
         Point point(pathPoint + _ptOrigin); // account for origin offset

         // if we have one, restore the previous pixel
         if (_lastActionContext.fHasData) {
            // we intentionally ignore the success of this call.  Failure will be
            // obvious on the display.
            _displaySurface.setPixelColor(_lastActionContext.point.getX(),
                                          _lastActionContext.point.getY(),
                                          _lastActionContext.crgbColor);
         }

         // cache the state of the next pixel
         {
            fSuccess = _displaySurface.getPixelColor(point.getX(), point.getY(),
                                                     _lastActionContext.crgbColor);
            EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));
            _lastActionContext.point        = point;
            _lastActionContext.ulLastStepMs = ulNowMs;
            _lastActionContext.fHasData     = true;
         }

         // render the cursor
         fSuccess = _displaySurface.setPixelColor(point.getX(), point.getY(),
                                                  _colorManager.getTransitionCursorColor());
         EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));
      } else {
         // we are at the end.
         if (_lastActionContext.fHasData) {
            _displaySurface.setPixelColor(_lastActionContext.point.getX(),
                                          _lastActionContext.point.getY(),
                                          _lastActionContext.crgbColor);
            _lastActionContext.fHasData = false;
         }
         _fIsDone = true;
      }
   }
End:
   return EHIsSuccess;
}
