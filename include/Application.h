#ifndef APPLICATION_H
#define APPLICATION_H

#include "CoordinateMapper.h"
#include "DisplaySurface.h"
#include "FiveBySevenDigitProvider.h"
#include "LEDBuffer.h"
#include <Arduino.h>

class Application {

 private:
   static constexpr uint8_t      _kBuzzerPin    = 15;
   static constexpr unsigned int _kMatrixWidth  = 32;
   static constexpr unsigned int _kMatrixHeight = 8;

   CoordinateMapper      _coordinateMapper;
   LEDBuffer             _ledBuffer;
   DisplaySurface        _displaySurface;
   ColorManager          _colorManager;
   const IDigitProvider& _iDigitProvider;

 public:
   Application(const IDigitProvider& iDigitProvider);

   void initialize();
   void tick();

 private:
};

#endif
