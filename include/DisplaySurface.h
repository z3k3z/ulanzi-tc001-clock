#ifndef DISPLAY_SURFACE_H
#define DISPLAY_SURFACE_H

#include "ColorManager.h"
#include "CoordinateMapper.h"
#include "LEDBuffer.h"

class DisplaySurface {
 private:
   static constexpr uint8_t _kDisplayPin = 32;

   CoordinateMapper& _coordinateMapper;
   LEDBuffer&        _ledBuffer;
   ColorManager      _colorManager;

 public:
   DisplaySurface(CoordinateMapper& coordinateMapper, LEDBuffer& ledBuffer) :
       _coordinateMapper(coordinateMapper), _ledBuffer(ledBuffer), _colorManager() {}

   bool initialize();
   bool setPixelColor(int iX, int iY, const CRGB& color);
   bool getPixelColor(int iX, int iY, CRGB& color);
   void clear(bool fWriteThrough = false);
   void show();

   ColorManager&       getColorManager() { return _colorManager; }
   const ColorManager& getColorManager() const { return _colorManager; }
};
#endif
