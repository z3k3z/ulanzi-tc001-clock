#ifndef APPLICATION_H
#define APPLICATION_H

#include "ColonSeparator.h"
#include "CoordinateMapper.h"
#include "DigitSlot.h"
#include "DisplaySurface.h"
#include "FiveBySevenDigitProvider.h"
#include "LEDBuffer.h"
#include "SerialTimeSyncProvider.h"
#include "ValueTracker.h"
#include <Arduino.h>

class Application {
 private:
   static constexpr uint8_t      _kBuzzerPin            = 15;
   static constexpr unsigned int _kMatrixWidth          = 32;
   static constexpr unsigned int _kMatrixHeight         = 8;
   static constexpr unsigned int _kNumDigits            = 4;
   static constexpr unsigned int _kSweepRateMs          = 15;
   static constexpr unsigned int _kInitialDigitVal      = 0;
   static constexpr unsigned int _kColonBlinkIntervalMs = 1000;

   CoordinateMapper       _coordinateMapper;
   LEDBuffer              _ledBuffer;
   DisplaySurface         _displaySurface;
   const IDigitProvider&  _iDigitProvider;
   DigitSlot              _digitSlots[_kNumDigits];
   ColonSeparator         _colonSeparator;
   ValueTracker           _valueTracker;
   SerialTimeSyncProvider _serialTimeSyncProvider;

 public:
   Application(const IDigitProvider& iDigitProvider);

   void initialize();
   void tick();

 private:
   bool _initializeDigitSlots();
   bool _getTimeAsInt(int& iValue);

   static const PixelGlyph& _getInitialGlyph(const IDigitProvider& iDigitProvider);
};

#endif
