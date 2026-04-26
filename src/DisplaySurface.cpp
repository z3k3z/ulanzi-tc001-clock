#include "DisplaySurface.h"
#include "errorh.h"
#include <FastLED.h>

bool DisplaySurface::initialize() {
   EHInitialize;
   const uint16_t uiLedCount = (uint16_t)_ledBuffer.getCount();
   CRGB*          pLEDs      = _ledBuffer.getBuffer();

   FastLED.addLeds<WS2812B, _kDisplayPin, GRB>(pLEDs, uiLedCount);
   FastLED.setBrightness(_kBrightness);
   FastLED.clear(true);

End:
   return EHIsSuccess;
}

bool DisplaySurface::setPixelColor(int iX, int iY, const CRGB& color) {
   EHInitialize;
   unsigned int uiIndex  = 0;
   CRGB*        pLED     = nullptr;
   bool         fSuccess = false;

   // obtain the pixel index from the coordinates
   fSuccess = _coordinateMapper.indexFromCoordinates(iX, iY, uiIndex);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(iX, iY));

   // get the pixel from the buffer
   fSuccess = _ledBuffer.getLEDAt(uiIndex, pLED);
   EHRaiseErrorWhenNotSuccess(fSuccess, uiIndex);

   // set the color
   *pLED = color;

End:
   return EHIsSuccess;
}

bool DisplaySurface::getPixelColor(int iX, int iY, CRGB& color) {
   EHInitialize;
   unsigned int uiIndex  = 0;
   CRGB*        pLED     = nullptr;
   bool         fSuccess = false;

   // obtain the pixel index from the coordinates
   fSuccess = _coordinateMapper.indexFromCoordinates(iX, iY, uiIndex);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(iX, iY));

   // get the pixel from the buffer
   fSuccess = _ledBuffer.getLEDAt(uiIndex, pLED);
   EHRaiseErrorWhenNotSuccess(fSuccess, uiIndex);

   // get the color of that pixel
   color = *pLED;

End:
   return EHIsSuccess;
}

void DisplaySurface::clear(bool fWriteThrough) {
   FastLED.clear(fWriteThrough);
}

void DisplaySurface::show() {
   FastLED.show();
}
