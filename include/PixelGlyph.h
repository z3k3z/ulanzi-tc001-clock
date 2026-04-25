#ifndef PIXEL_GLYPH_H
#define PIXEL_GLYPH_H
#include "ColorManager.h"
#include "DisplaySurface.h"

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

   bool draw(DisplaySurface& displaySurface, int iX, int iY,
             const ColorManager& colorManager) const;
};
#endif
