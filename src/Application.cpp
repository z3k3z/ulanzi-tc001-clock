#include "Application.h"
#include "errorh.h"
#include <FastLED.h>

Application::Application()
    : _coordinateMapper(_kMatrixWidth, _kMatrixHeight), _ledBuffer(),
      _displaySurface(_coordinateMapper, _ledBuffer), _uiCurrentX(0), _uiCurrentY(0),
      _ulLastStepMs(0) {
}

void Application::initialize() {
   const uint16_t uiLedCount = (uint16_t)_ledBuffer.getCount();
   CRGB*          pLeds      = _ledBuffer.getBuffer();

   Serial.begin(115200);
   delay(500);

   pinMode(_kBuzzerPin, INPUT_PULLDOWN);

   FastLED.addLeds<WS2812B, _kDisplayPin, GRB>(pLeds, uiLedCount);
   FastLED.setBrightness(_kBrightness);
   FastLED.clear(true);

   _ulLastStepMs = millis();

   renderCurrentPixel();

   Serial.println("Application initialized");
}

void Application::tick() {
   unsigned long ulNowMs = millis();

   if ((ulNowMs - _ulLastStepMs) < _kStepIntervalMs) {
      return;
   }

   _ulLastStepMs = ulNowMs;

   advanceLogicalCoordinate();

   if (!renderCurrentPixel()) {
      Serial.println("Render failure");
   }
}

void Application::advanceLogicalCoordinate() {
   ++_uiCurrentX;

   if (_uiCurrentX >= _kMatrixWidth) {
      _uiCurrentX = 0;
      ++_uiCurrentY;

      if (_uiCurrentY >= _kMatrixHeight) {
         _uiCurrentY = 0;
      }
   }
}

bool Application::renderCurrentPixel() {
   EHInitialize;
   bool fSuccess = false;

   FastLED.clear(false);

   for (unsigned int uiTrailOffset = 0; uiTrailOffset < _kTrailLength; ++uiTrailOffset) {
      unsigned int uiLinearPosition = (_uiCurrentY * _kMatrixWidth) + _uiCurrentX;

      unsigned int uiTrailPosition =
          (uiLinearPosition + (_kMatrixWidth * _kMatrixHeight) - uiTrailOffset) %
          (_kMatrixWidth * _kMatrixHeight);

      unsigned int uiTrailX = uiTrailPosition % _kMatrixWidth;
      unsigned int uiTrailY = uiTrailPosition / _kMatrixWidth;

      uint8_t uiBrightness = (uint8_t)(255 / (uiTrailOffset + 1));

      CRGB color = CRGB(uiBrightness, 0, 0);

      fSuccess = _displaySurface.setPixelColor((int)uiTrailX, (int)uiTrailY, color);
      EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(uiTrailX, uiTrailY));
   }

   FastLED.show();

End:
   return EHIsSuccess;
}
