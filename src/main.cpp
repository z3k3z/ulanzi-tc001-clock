#include <Arduino.h>
#include <FastLED.h>
#include "CoordinateMapper.h"

static const uint8_t kDisplayPin = 32;
static const uint16_t kLedCount = 256;
static const uint8_t kBrightness = 16;
static const uint8_t kBuzzerPin = 15;

static const unsigned int kMatrixWidth = 32;
static const unsigned int kMatrixHeight = 8;
static const unsigned long kStepIntervalMs = 50;

static CRGB gLeds[kLedCount];
static CoordinateMapper gCoordinateMapper(kMatrixWidth, kMatrixHeight);

static unsigned int gUiCurrentX = 0;
static unsigned int gUiCurrentY = 0;
static unsigned long gUlLastStepMs = 0;

static void clearDisplay()
{
    FastLED.clear(false);
}

static void advanceLogicalCoordinate()
{
    ++gUiCurrentX;

    if (gUiCurrentX >= kMatrixWidth) {
        gUiCurrentX = 0;
        ++gUiCurrentY;

        if (gUiCurrentY >= kMatrixHeight) {
            gUiCurrentY = 0;
        }
    }
}

static bool renderCurrentPixel()
{
    unsigned int uiIndex = 0;

    if (!gCoordinateMapper.indexFromCoordinates(
            (int)gUiCurrentX,
            (int)gUiCurrentY,
            uiIndex)) {
        return false;
    }

    clearDisplay();
    gLeds[uiIndex] = CRGB::Red;
    FastLED.show();

    return true;
}

void setup()
{
    Serial.begin(115200);
    delay(500);

    pinMode(kBuzzerPin, INPUT_PULLDOWN);

    FastLED.addLeds<WS2812B, kDisplayPin, GRB>(gLeds, kLedCount);
    FastLED.setBrightness(kBrightness);
    FastLED.clear(true);

    gUlLastStepMs = millis();

    renderCurrentPixel();

    Serial.println("CoordinateMapper walk demo started");
}

void loop()
{
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
