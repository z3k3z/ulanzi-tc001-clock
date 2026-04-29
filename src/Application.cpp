#include "Application.h"
#include "PointPath.h"
#include "errorh.h"

// clang-format off

static const Point kPointPath5x7RandomPoints[] = {
   Point(2, 3), Point(4, 0), Point(1, 6), Point(0, 2), Point(3, 5),
   Point(1, 1), Point(4, 4), Point(2, 0), Point(0, 6), Point(3, 2),
   Point(4, 1), Point(2, 5), Point(1, 3), Point(0, 0), Point(3, 6),
   Point(4, 2), Point(2, 1), Point(1, 5), Point(0, 4), Point(3, 0),
   Point(4, 6), Point(2, 4), Point(1, 0), Point(0, 5), Point(3, 3),
   Point(4, 3), Point(2, 2), Point(1, 4), Point(0, 1), Point(3, 1),
   Point(4, 5), Point(2, 6), Point(1, 2), Point(0, 3), Point(3, 4),
};

static const PointPath kPointPath5x7Random(
   kPointPath5x7RandomPoints,
   sizeof(kPointPath5x7RandomPoints) / sizeof(kPointPath5x7RandomPoints[0])
);
// clang-format on

Application::Application(const IDigitProvider& iDigitProvider) :
    _coordinateMapper(_kMatrixWidth, _kMatrixHeight),
    _ledBuffer(),
    _displaySurface(_coordinateMapper, _ledBuffer),
    _colorManager(),
    _iDigitProvider(iDigitProvider),
    _valueTracker() {

   _valueTracker.setInitialValue(0);
   _digits[3].ptOrigin = Point(1, 0);
   _digits[2].ptOrigin = Point(8, 0);
   _digits[1].ptOrigin = Point(15, 0);
   _digits[0].ptOrigin = Point(22, 0);
}

void Application::initialize() {
   EHInitialize;
   bool fSuccess;

   Serial.begin(115200);
   delay(500);

   pinMode(_kBuzzerPin, INPUT_PULLDOWN);

   _colorManager.setTheme(ColorTheme::WarmBusMarquee);
   _displaySurface.initialize();
   _displaySurface.clear();
   fSuccess = _renderInitialDisplay();
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
   bool fSuccess    = false;
   bool fHasChanged = false;
   int  iTimeValue  = 0;

   fSuccess = _getTimeAsInt(iTimeValue);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

   fSuccess = _valueTracker.updateValue(iTimeValue, fHasChanged);
   EHRaiseErrorWhenNotSuccess(fSuccess, iTimeValue);

   if (fHasChanged) {
      for (unsigned int ui = 0; ui < _kNumDigits; ui++) {
         int iNewValue;
         fSuccess = _valueTracker.queryDigitNewValue(ui, iNewValue, fHasChanged);
         EHRaiseErrorWhenNotSuccess(fSuccess, ui);
         if (fHasChanged) {
            fSuccess = _renderDigitFor(ui, iNewValue);
            EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(ui, iNewValue));
         }
      }
      _displaySurface.show();
   }

End:
   if (EHErrorRaised) {
      EHEmitMsg;
   }
}

bool Application::_renderDigitFor(unsigned int uiDigit, unsigned int uiValue) {
   EHInitialize;
   bool fSuccess;

   EHRaiseErrorWhen((_kNumDigits <= uiDigit), uiDigit);
   {
      const PixelGlyph* pPixelGlyph = nullptr;

      fSuccess = _iDigitProvider.getDigitFor(uiValue, pPixelGlyph);
      EHRaiseErrorWhenNotSuccess(fSuccess, uiValue);
      EHRaiseErrorWhen(nullptr == pPixelGlyph, uiValue);

      fSuccess = pPixelGlyph->draw(_displaySurface, _digits[uiDigit].ptOrigin.getX(),
                                   _digits[uiDigit].ptOrigin.getY(), _colorManager);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool Application::_renderInitialDisplay() {
   EHInitialize;
   bool fSuccess = false;

   for (unsigned int ui = 0; ui < _kNumDigits; ui++) {
      fSuccess = _renderDigitFor(ui, 0);
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

   int iHour    = tmNow.tm_hour % 10;
   int iMinute  = tmNow.tm_min;
   int iSecondT = tmNow.tm_sec / 10;
   iValue       = (iHour * 1000) + (iMinute * 10) + iSecondT;

   return true;
}
