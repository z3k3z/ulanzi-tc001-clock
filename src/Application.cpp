#include "Application.h"
#include "PointPath.h"
#include "errorh.h"
#include <WiFi.h>

// clang-format off
static const Point kPointPath5x8RandomPoints[] = {
   Point(2, 3), Point(4, 0), Point(1, 7), Point(0, 2), Point(3, 5),
   Point(1, 1), Point(4, 4), Point(2, 0), Point(0, 7), Point(3, 2),
   Point(4, 1), Point(2, 6), Point(1, 3), Point(0, 0), Point(3, 7),
   Point(4, 2), Point(2, 1), Point(1, 6), Point(0, 4), Point(3, 0),
   Point(4, 7), Point(2, 5), Point(1, 0), Point(0, 6), Point(3, 3),
   Point(4, 3), Point(2, 2), Point(1, 5), Point(0, 1), Point(3, 1),
   Point(4, 6), Point(2, 7), Point(1, 2), Point(0, 3), Point(3, 4),
   Point(4, 5), Point(2, 4), Point(1, 4), Point(0, 5), Point(3, 6),
};

static const PointPath kPointPath5x8Random(
   kPointPath5x8RandomPoints,
   sizeof(kPointPath5x8RandomPoints) / sizeof(kPointPath5x8RandomPoints[0])
);
// clang-format on

Application::Application(const IDigitProvider& iDigitProvider,
                         const IDigitProvider& iDigitProviderAlt) :
    _coordinateMapper(_kMatrixWidth, _kMatrixHeight),
    _ledBuffer(),
    _displaySurface(_coordinateMapper, _ledBuffer),
    _iDigitProvider(iDigitProvider),
    _iDigitProviderAlt(iDigitProviderAlt),
    _digitSlots{
        DigitSlot(_displaySurface, _iDigitProvider, kPointPath5x8Random, _kSweepRateMs,
                  Point(27, 0), _getInitialGlyph(_iDigitProvider)),
        DigitSlot(_displaySurface, _iDigitProvider, kPointPath5x8Random, _kSweepRateMs,
                  Point(19, 0), _getInitialGlyph(_iDigitProvider)),
        DigitSlot(_displaySurface, _iDigitProvider, kPointPath5x8Random, _kSweepRateMs, Point(9, 0),
                  _getInitialGlyph(_iDigitProvider)),
        DigitSlot(_displaySurface, _iDigitProviderAlt, kPointPath5x8Random, _kSweepRateMs,
                  Point(1, 0), _getInitialGlyph(_iDigitProviderAlt)),
    },
    _colonSeparator(Point(16, 0), _kMatrixHeight, _kColonBlinkIntervalMs),
    _valueTracker(),
    _serialTimeSyncProvider() {

   _valueTracker.setInitialValue(0);
}

void Application::initialize() {
   EHInitialize;
   bool fSuccess = false;

   Serial.begin(115200);
   delay(500);

   pinMode(_kBuzzerPin, INPUT_PULLDOWN);

   fSuccess = _serialTimeSyncProvider.initialize();
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

   _displaySurface.getColorManager().setTheme(ColorTheme::WarmBusMarquee);
   _displaySurface.initialize();
   _displaySurface.clear();

   fSuccess = _initializeDigitSlots();
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

   fSuccess = _colonSeparator.initialize(_displaySurface);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

   _displaySurface.show();

   Serial.println("Application initialized");

End:
   if (EHErrorRaised) {
      EHEmitMsg;
   }
}

void Application::tick() {
   EHInitialize;
   bool fSuccess           = false;
   bool fHasChanged        = false;
   bool fDisplayDirty      = false;
   bool fColonDisplayDirty = false;
   int  iTimeValue         = 0;

   // check if we have a pending time-sync event
   {
      bool fTimeWasUpdated = false;
      fSuccess             = _serialTimeSyncProvider.handleTick(fTimeWasUpdated);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);
   }

   fSuccess = _getTimeAsInt(iTimeValue);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

   fSuccess = _valueTracker.updateValue(iTimeValue, fHasChanged);
   EHRaiseErrorWhenNotSuccess(fSuccess, iTimeValue);

   if (fHasChanged) {
      for (unsigned int ui = 0; ui < _kNumDigits; ui++) {
         int iNewValue = 0;

         fSuccess = _valueTracker.queryDigitNewValue(ui, iNewValue, fHasChanged);
         EHRaiseErrorWhenNotSuccess(fSuccess, ui);

         if (fHasChanged) {
            fSuccess = _digitSlots[ui].beginTransitionTo((unsigned int)iNewValue);
            EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(ui, iNewValue));

            fDisplayDirty = true;
         }
      }
   }

   for (unsigned int ui = 0; ui < _kNumDigits; ui++) {
      DigitSlotState stateBeforeTick = _digitSlots[ui].getCurrentState();

      fSuccess = _digitSlots[ui].handleTick();
      EHRaiseErrorWhenNotSuccess(fSuccess, ui);

      if (stateBeforeTick == DigitSlotState::Transitioning) {
         fDisplayDirty = true;
      }
   }

   fSuccess = _colonSeparator.handleTick(_displaySurface, fColonDisplayDirty);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);
   fDisplayDirty = fDisplayDirty || fColonDisplayDirty;

   if (fDisplayDirty) {
      _displaySurface.show();
   }

End:
   if (EHErrorRaised) {
      EHEmitMsg;
   }
}

bool Application::_initializeDigitSlots() {
   EHInitialize;
   bool fSuccess = false;

   for (unsigned int ui = 0; ui < _kNumDigits; ui++) {
      fSuccess = _digitSlots[ui].initialize(_kInitialDigitVal);
      EHRaiseErrorWhenNotSuccess(fSuccess, ui);
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool Application::_getTimeAsInt(int& iValue) {
   time_t    now = time(nullptr);
   struct tm tmNow;
   localtime_r(&now, &tmNow);

   int iHour24 = tmNow.tm_hour;
   int iMinute = tmNow.tm_min;

   // convert to 12 hour format
   int iHour12 = iHour24 % 12;
   if (0 == iHour12) {
      iHour12 = 12;
   }

   iValue = (iHour12 * 100) + iMinute;

   return true;
}

bool Application::_configurePower() {
   EHInitialize;

   WiFi.mode(WIFI_OFF); // disable WIFI
   btStop();            // disable bluetooth

   return EHIsSuccess;
}

const PixelGlyph& Application::_getInitialGlyph(const IDigitProvider& iDigitProvider) {
   const PixelGlyph* pPixelGlyph = nullptr;

   iDigitProvider.getDigitFor(_kInitialDigitVal, pPixelGlyph);

   return *pPixelGlyph;
}
