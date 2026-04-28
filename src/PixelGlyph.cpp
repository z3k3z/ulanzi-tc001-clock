#include "PixelGlyph.h"
#include "PointIterator.h"
#include "Rectangle.h"
#include "errorh.h"

bool PixelGlyph::draw(DisplaySurface& displaySurface, const Point& ptOrigin,
                      const ColorManager& colorManager) const {
   EHInitialize;

   Rectangle     glyphBounds(0, 0, _uiWidth, _uiHeight);
   PointIterator pointIterator(glyphBounds, IterationOrder::XMajor);
   Point         point(0, 0);
   bool          fSuccess = false;

   // iterate over the glyph pixels
   while (pointIterator.next(point)) {
      fSuccess = drawPixelForPoint(displaySurface, point, ptOrigin, colorManager);
      EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(point.getX(), point.getY()));
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool PixelGlyph::getPixelColorForPoint(const Point& ptLocal, const ColorManager& colorManager,
                                       CRGB& crgbPixelColor) const {
   EHInitialize;
   int iXOffset = ptLocal.getX();
   int iYOffset = ptLocal.getY();

   EHRaiseErrorWhen((iXOffset < 0 || iYOffset < 0 || (unsigned int)iXOffset >= _uiWidth ||
                     (unsigned int)iYOffset >= _uiHeight),
                    EH_PACK_INT16_TO_LONG(iXOffset, iYOffset));
   EHRaiseErrorWhen(nullptr == _puiRows, 0);
   {
      // access the bit that encodes the 'active' state of this pixel.  An element
      // within _puiRows encodes the bits for an entire row.  Our left to right rendering
      // pulls high order to low order bits, respectfully.
      unsigned int uiBitOffset    = _uiWidth - 1 - (unsigned int)iXOffset;
      bool         fPixelIsActive = ((_puiRows[iYOffset] & (1 << uiBitOffset)) != 0);

      crgbPixelColor =
          fPixelIsActive ? colorManager.getActiveColor() : colorManager.getInactiveColor();
   }

End:
   if (EHErrorRaised) {
      char szContext[64];
      snprintf(szContext, sizeof(szContext), "puiRows:%p width:%d height:%d", (void*)_puiRows,
               _uiWidth, _uiHeight);
      EHSetAdditionalContext(szContext);
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool PixelGlyph::drawPixelForPoint(DisplaySurface& displaySurface, const Point& ptLocal,
                                   const Point& ptOrigin, const ColorManager& colorManager) const {
   EHInitialize;
   bool fSuccess;
   CRGB color;

   fSuccess = getPixelColorForPoint(ptLocal, colorManager, color);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(ptLocal.getX(), ptLocal.getY()));
   {
      Point ptDisplay = ptLocal + ptOrigin;
      fSuccess        = displaySurface.setPixelColor(ptDisplay.getX(), ptDisplay.getY(), color);
      EHRaiseErrorWhenNotSuccess(fSuccess,
                                 EH_PACK_INT16_TO_LONG(ptDisplay.getX(), ptDisplay.getY()));
   }

End:
   if (EHErrorRaised) {
      char szContext[64];
      snprintf(szContext, sizeof(szContext), "Local X:%d, Y:%d  Origin X:%d, Y:%d", ptLocal.getX(),
               ptLocal.getY(), ptOrigin.getX(), ptOrigin.getY());
      EHSetAdditionalContext(szContext);
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
