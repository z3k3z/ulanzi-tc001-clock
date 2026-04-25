#include "PixelGlyph.h"
#include "errorh.h"

bool PixelGlyph::draw(DisplaySurface& displaySurface, int iX, int iY, const CRGB& color) const {
   EHInitialize;
   CRGB inactiveColor(8, 0, 0);

   for (int y = 0; y < _uiHeight; y++) {
      for (int x = _uiWidth; x > 0; x--) {
         const CRGB& c = (_puiRows[y] & (1 << (x - 1))) ? color : inactiveColor;
         displaySurface.setPixelColor(iX + (_uiWidth - x), iY + y, c);
      }
   }

End:
   return EHIsSuccess;
}
