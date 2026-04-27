#include "SimpleSweep.h"
#include "errorh.h"

bool SimpleSweep::leavingFromPoint(const Point& point) {
   EHInitialize;
   bool fSuccess = false;

   // if we have one, restore the previous pixel
   if (_lastPointDataCache.fHasData) {
      Point ptWithOffset(point + _ptOrigin); // account for origin offset
      fSuccess = _displaySurface.setPixelColor(ptWithOffset.getX(), ptWithOffset.getY(),
                                               _lastPointDataCache.crgbColor);
      EHRaiseErrorWhenNotSuccess(fSuccess,
                                 EH_PACK_INT16_TO_LONG(ptWithOffset.getX(), ptWithOffset.getY()));
      _lastPointDataCache.fHasData = false;
   }

End:
   return EHIsSuccess;
}

bool SimpleSweep::landingOnPoint(const Point& point) {
   EHInitialize;
   bool  fSuccess = false;
   Point ptWithOffset(point + _ptOrigin); // account for origin offset

   // cache the state of the next pixel
   fSuccess = _displaySurface.getPixelColor(ptWithOffset.getX(), ptWithOffset.getY(),
                                            _lastPointDataCache.crgbColor);
   EHRaiseErrorWhenNotSuccess(fSuccess,
                              EH_PACK_INT16_TO_LONG(ptWithOffset.getX(), ptWithOffset.getY()));
   _lastPointDataCache.fHasData = true;

   // draw the cursor
   fSuccess = _displaySurface.setPixelColor(ptWithOffset.getX(), ptWithOffset.getY(),
                                            _colorManager.getTransitionCursorColor());
   EHRaiseErrorWhenNotSuccess(fSuccess,
                              EH_PACK_INT16_TO_LONG(ptWithOffset.getX(), ptWithOffset.getY()));

End:
   return EHIsSuccess;
}
