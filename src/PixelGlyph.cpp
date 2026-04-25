#include "PixelGlyph.h"
#include "errorh.h"

bool PixelGlyph::draw(DisplaySurface& displaySurface, int iX, int iY,
                      const ColorManager& colorManager) const {
   EHInitialize;

   for (int y = 0; y < _uiHeight; y++) {
      for (int x = _uiWidth; x > 0; x--) {
         const CRGB& c        = (_puiRows[y] & (1 << (x - 1))) ? colorManager.getActiveColor()
                                                               : colorManager.getInactiveColor();
         bool        fSuccess = displaySurface.setPixelColor(iX + (_uiWidth - x), iY + y, c);
         EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(iX + (_uiWidth - x), iY + y));
      }
   }

End:
   return EHIsSuccess;
}
