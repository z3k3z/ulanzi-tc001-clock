#ifndef DISPLAY_SURFACE_H
#define DISPLAY_SURFACE_H

#include "CoordinateMapper.h"
#include "LEDBuffer.h"

class DisplaySurface {
 private:
   CoordinateMapper& _coordinateMapper;
   LEDBuffer&        _LEDBuffer;

 public:
   DisplaySurface(CoordinateMapper& coordinateMapper, LEDBuffer& ledBuffer)
       : _coordinateMapper(coordinateMapper), _LEDBuffer(ledBuffer) {}

   bool setPixelColor(int iX, int iY, const CRGB& color);
};
#endif
