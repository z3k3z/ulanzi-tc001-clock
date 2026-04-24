#ifndef DISPLAY_SURFACE_H
#define DISPLAY_SURFACE_H

#include "CoordinateMapper.h"
#include "LEDBuffer.h"

class DisplaySurface {
 private:
   static constexpr uint8_t _kDisplayPin = 32;
   static constexpr uint8_t _kBrightness = 16;

   CoordinateMapper& _coordinateMapper;
   LEDBuffer&        _ledBuffer;

 public:
   DisplaySurface(CoordinateMapper& coordinateMapper, LEDBuffer& ledBuffer)
       : _coordinateMapper(coordinateMapper), _ledBuffer(ledBuffer) {}

   bool initialize();
   bool setPixelColor(int iX, int iY, const CRGB& color);
   void clear(bool fWriteThrough = false);
   void show();
};
#endif
