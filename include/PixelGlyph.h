#ifndef PIXEL_GLYPH_H
#define PIXEL_GLYPH_H
#include "DisplaySurface.h"
#include "Point.h"

class PixelGlyph {

 private:
   const uint8_t* _puiRows;
   unsigned int   _uiWidth;
   unsigned int   _uiHeight;

 public:
   PixelGlyph(const uint8_t* puiRows, unsigned int uiWidth, unsigned int uiHeight) {
      _puiRows  = puiRows;
      _uiWidth  = uiWidth;
      _uiHeight = uiHeight;
   }

   bool draw(DisplaySurface& displaySurface, int iX, int iY) const {
      Point pt(iX, iY);
      return draw(displaySurface, pt);
   }

   bool draw(DisplaySurface& displaySurface, const Point& ptOrigin) const;
   bool getPixelColorForPoint(DisplaySurface& displaySurface, const Point& ptLocal,
                              CRGB& crgbPixelColor) const;
   bool drawPixelForPoint(DisplaySurface& displaySurface, const Point& ptLocal,
                          const Point& ptOrigin, const CRGB* pOverrideColor = nullptr) const;
};
#endif
