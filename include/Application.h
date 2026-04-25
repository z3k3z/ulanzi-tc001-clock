#ifndef APPLICATION_H
#define APPLICATION_H

#include "CoordinateMapper.h"
#include "DisplaySurface.h"
#include "LEDBuffer.h"
#include "PixelGlyph.h"
#include <Arduino.h>

class Application {
 private:
   static constexpr uint8_t       _kBuzzerPin      = 15;
   static constexpr unsigned int  _kMatrixWidth    = 32;
   static constexpr unsigned int  _kMatrixHeight   = 8;
   static constexpr unsigned long _kStepIntervalMs = 50;
   static constexpr unsigned int  _kTrailLength    = 6;
   static const uint8_t           _kDigit0Rows[7];
   static const uint8_t           _kDigit1Rows[7];

   CoordinateMapper _coordinateMapper;
   LEDBuffer        _ledBuffer;
   DisplaySurface   _displaySurface;
   PixelGlyph       _digit0Glyph;
   PixelGlyph       _digit1Glyph;
   ColorManager     _colorManager;

   unsigned int  _uiCurrentX;
   unsigned int  _uiCurrentY;
   unsigned long _ulLastStepMs;

   void advanceLogicalCoordinate();
   bool renderCurrentPixel();

 public:
   Application();

   void initialize();
   void tick();
};

#endif
