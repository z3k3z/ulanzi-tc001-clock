#include "PixelGlyph.h"
#include "PointIterator.h"
#include "Rectangle.h"
#include "errorh.h"

bool PixelGlyph::draw(DisplaySurface& displaySurface, const Point& ptOrigin,
                      const ColorManager& colorManager) const {
   EHInitialize;

   Rectangle     glyphBounds(ptOrigin.getX(), ptOrigin.getY(), _uiWidth, _uiHeight);
   PointIterator pointIterator(glyphBounds, IterationOrder::XMajor);
   Point         point(0, 0);
   bool          fSuccess = false;

   // iterate over the glyph pixels
   while (pointIterator.next(point)) {
      int iXOffset = point.getX() - ptOrigin.getX(); // offset from X origin
      int iYOffset = point.getY() - ptOrigin.getY(); // offset from Y origin

      // access the bit that encodes the 'active' state of this pixel.  An element
      // within _puiRows encodes the bits for an entire row.  Our left to right rendering
      // pulls high order to low order bits, respectfully.
      unsigned int uiBitOffset    = _uiWidth - 1 - (unsigned int)iXOffset;
      bool         fPixelIsActive = ((_puiRows[iYOffset] & (1 << uiBitOffset)) != 0);

      const CRGB& color =
          fPixelIsActive ? colorManager.getActiveColor() : colorManager.getInactiveColor();

      fSuccess = displaySurface.setPixelColor(point.getX(), point.getY(), color);
      EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));
   }

End:
   return EHIsSuccess;
}
