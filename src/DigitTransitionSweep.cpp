#include "DigitTransitionSweep.h"
#include "errorh.h"

bool DigitTransitionSweep::leavingFromPoint(const Point& point) {
   EHInitialize;
   bool fSuccess = false;

   EHRaiseErrorWhen(nullptr == _pToGlyph, 0);
   fSuccess = _pToGlyph->drawPixelForPoint(_displaySurface, point, _ptOrigin);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool DigitTransitionSweep::landingOnPoint(const Point& point) {
   EHInitialize;
   bool  fSuccess = false;
   Point ptWithOffset(point + _ptOrigin); // account for origin offset

   // draw the cursor
   fSuccess =
       _displaySurface.setPixelColor(ptWithOffset.getX(), ptWithOffset.getY(),
                                     _displaySurface.getColorManager().getTransitionCursorColor());
   EHRaiseErrorWhenNotSuccess(fSuccess,
                              EH_PACK_INT16_TO_LONG(ptWithOffset.getX(), ptWithOffset.getY()));

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
