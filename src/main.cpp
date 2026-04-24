#include "CoordinateMapper.h"
#include "DisplaySurface.h"
#include "LEDBuffer.h"
#include "errorh.h"
#include <Arduino.h>
#include <FastLED.h>

static const uint8_t kDisplayPin = 32;
static const uint8_t kBrightness = 16;
static const uint8_t kBuzzerPin  = 15;

static const unsigned int  kMatrixWidth    = 32;
static const unsigned int  kMatrixHeight   = 8;
static const unsigned long kStepIntervalMs = 50;

static CoordinateMapper gCoordinateMapper(kMatrixWidth, kMatrixHeight);
static LEDBuffer        gLEDBuffer;
static DisplaySurface   gDisplaySurface(gCoordinateMapper, gLEDBuffer);

static unsigned int  gUiCurrentX   = 0;
static unsigned int  gUiCurrentY   = 0;
static unsigned long gUlLastStepMs = 0;

static void clearDisplay() {
   FastLED.clear(false);
}

static void advanceLogicalCoordinate() {
   ++gUiCurrentX;

   if (gUiCurrentX >= kMatrixWidth) {
      gUiCurrentX = 0;
      ++gUiCurrentY;

      if (gUiCurrentY >= kMatrixHeight) {
         gUiCurrentY = 0;
      }
   }
}

static bool renderCurrentPixel() {
   EHInitialize;
   bool fSuccess = false;

   clearDisplay();
   fSuccess = gDisplaySurface.setPixelColor((int)gUiCurrentX, (int)gUiCurrentY, CRGB::Red);
   EHRaiseErrorWhenNotSuccess(fSuccess, EH_PACK_INT16_TO_LONG(gUiCurrentX, gUiCurrentY));

   FastLED.show();

End:
   return EHIsSuccess;
}

void setup() {
   const uint16_t ledCount = gLEDBuffer.getCount();
   CRGB*          leds     = gLEDBuffer.getBuffer();

   Serial.begin(115200);
   delay(500);

   pinMode(kBuzzerPin, INPUT_PULLDOWN);

   FastLED.addLeds<WS2812B, kDisplayPin, GRB>(leds, ledCount);
   FastLED.setBrightness(kBrightness);
   FastLED.clear(true);

   gUlLastStepMs = millis();

   renderCurrentPixel();

   Serial.println("CoordinateMapper walk demo started");
}

void loop() {
   unsigned long ulNowMs = millis();

   if ((ulNowMs - gUlLastStepMs) < kStepIntervalMs) {
      return;
   }

   gUlLastStepMs = ulNowMs;

   advanceLogicalCoordinate();

   if (!renderCurrentPixel()) {
      Serial.println("CoordinateMapper failure");
   }
}
