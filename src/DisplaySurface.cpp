#include "DisplaySurface.h"
#include "errorh.h"

bool DisplaySurface::setPixelColor(int iX, int iY, const CRGB& color) {
   EHInitialize;
   unsigned int uiIndex  = 0;
   CRGB*        pLED     = nullptr;
   bool         fSuccess = false;

   // obtain the pixel index from the coordinates
   fSuccess = _coordinateMapper.indexFromCoordinates(iX, iY, uiIndex);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(iX, iY));

   // get the pixel from the buffer
   fSuccess = _LEDBuffer.getLEDAt(uiIndex, pLED);
   EHRaiseErrorWhenNotSuccess(fSuccess, uiIndex);

   // set the color
   *pLED = color;

End:
   return EHIsSuccess;
}
