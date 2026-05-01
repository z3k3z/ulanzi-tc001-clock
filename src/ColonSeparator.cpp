#include "ColonSeparator.h"
#include "errorh.h"
#include <Arduino.h>

bool ColonSeparator::initialize(DisplaySurface& displaySurface) {
   EHInitialize;
   bool fSuccess = false;

   _fIsDotActive    = true;
   _fHasTickContext = false;
   _ulLastToggleMs  = 0;

   fSuccess = _render(displaySurface);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool ColonSeparator::handleTick(DisplaySurface& displaySurface, bool& fDisplayDirty) {
   EHInitialize;
   bool          fSuccess = false;
   unsigned long ulNowMs  = millis();

   fDisplayDirty = false;

   if (!_fHasTickContext) {
      _ulLastToggleMs  = ulNowMs;
      _fHasTickContext = true;
   } else if ((ulNowMs - _ulLastToggleMs) >= _uiBlinkIntervalMs) {
      _fIsDotActive   = !_fIsDotActive;
      _ulLastToggleMs = ulNowMs;

      fSuccess = _render(displaySurface);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);

      fDisplayDirty = true;
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool ColonSeparator::_render(DisplaySurface& displaySurface) {
   EHInitialize;
   bool fSuccess = false;

   for (unsigned int ui = 0; ui < _uiHeight; ui++) {
      bool fIsDotRow = ((2 == ui) || (5 == ui));

      CRGB color = (fIsDotRow && _fIsDotActive)
                       ? displaySurface.getColorManager().getActiveColor()
                       : displaySurface.getColorManager().getInactiveColor();

      fSuccess = displaySurface.setPixelColor(_ptOrigin.getX(), _ptOrigin.getY() + (int)ui, color);
      EHRaiseErrorWhenNotSuccess(
          fSuccess, EH_PACK_INT16_TO_LONG(_ptOrigin.getX(), _ptOrigin.getY() + (int)ui));
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
