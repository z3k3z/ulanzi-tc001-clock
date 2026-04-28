#ifndef APPLICATION_H
#define APPLICATION_H

#include "CoordinateMapper.h"
#include "DigitTransitionSweep.h"
#include "DisplaySurface.h"
#include "FiveBySevenDigitProvider.h"
#include "LEDBuffer.h"
#include "PixelSweeper.h"
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
   static const DigitDescriptor  _kDigitDescriptors[4];

   CoordinateMapper      _coordinateMapper;
   LEDBuffer             _ledBuffer;
   DisplaySurface        _displaySurface;
   ColorManager          _colorManager;
   PixelSweeper*         _pixelSweeper;
   PixelGlyph            _pixelGlyphNull;
   DigitTransitionSweep  _digitTransitionSweep;
   const IDigitProvider& _iDigitProvider;
   unsigned int          _currentDigit;

 public:
   Application(const IDigitProvider& iDigitProvider);

   void initialize();
   void tick();

 private:
   bool renderThemeZeros();
};

#endif
