#ifndef APPLICATION_H
#define APPLICATION_H

#include "CoordinateMapper.h"
#include "DigitTransitionSweep.h"
#include "DisplaySurface.h"
#include "LEDBuffer.h"
#include "PixelGlyph.h"
#include "PixelSweeper.h"
#include "SimpleSweep.h"
#include <Arduino.h>

class Application {
 private:
   struct DigitDescriptor {
      Point      pointOrigin;
      ColorTheme colorTheme;
   };

 private:
   static constexpr uint8_t      _kBuzzerPin    = 15;
   static constexpr unsigned int _kMatrixWidth  = 32;
   static constexpr unsigned int _kMatrixHeight = 8;
   static constexpr unsigned int _kGlyphWidth   = 5;
   static constexpr unsigned int _kGlyphHeight  = 7;
   static const uint8_t          _kDigit0Rows[7];
   static const uint8_t          _kDigit1Rows[7];
   static const DigitDescriptor  _kDigitDescriptors[4];

   CoordinateMapper     _coordinateMapper;
   LEDBuffer            _ledBuffer;
   DisplaySurface       _displaySurface;
   PixelGlyph           _digit0Glyph;
   PixelGlyph           _digit1Glyph;
   ColorManager         _colorManager;
   SimpleSweep          _simpleSweep;
   PixelSweeper*        _pixelSweeper;
   DigitTransitionSweep _digitTransitionSweep;
   unsigned int         _currentDigit;

 public:
   Application();

   void initialize();
   void tick();

 private:
   bool renderThemeZeros();
};

#endif
