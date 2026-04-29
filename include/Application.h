#ifndef APPLICATION_H
#define APPLICATION_H

#include "CoordinateMapper.h"
#include "DisplaySurface.h"
#include "FiveBySevenDigitProvider.h"
#include "LEDBuffer.h"
#include "ValueTracker.h"
#include <Arduino.h>

class Application {
 private:
   struct DigitDescriptor {
      Point ptOrigin;
   };

 private:
   static constexpr uint8_t      _kBuzzerPin    = 15;
   static constexpr unsigned int _kMatrixWidth  = 32;
   static constexpr unsigned int _kMatrixHeight = 8;
   static constexpr unsigned int _kNumDigits    = 4;

   DigitDescriptor       _digits[_kNumDigits];
   CoordinateMapper      _coordinateMapper;
   LEDBuffer             _ledBuffer;
   DisplaySurface        _displaySurface;
   const IDigitProvider& _iDigitProvider;
   ValueTracker          _valueTracker;

 public:
   Application(const IDigitProvider& iDigitProvider);

   void initialize();
   void tick();

 private:
   bool _renderDigitFor(unsigned int uiDigit, unsigned int uiValue);
   bool _renderInitialDisplay();
   bool _getTimeAsInt(int& iValue);
};

#endif
