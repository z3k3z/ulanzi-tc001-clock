#include <Arduino.h>
#include <FastLED.h>

static const uint8_t kDisplayPin = 32;
static const uint16_t kLedCount = 256;
static const uint8_t kBrightness = 16;
static const uint8_t kBuzzerPin = 15;

static CRGB gLeds[kLedCount];

void setup()
{
    Serial.begin(115200);
    delay(500);
    Serial.println("Starting TC001 matrix test");

    // Silence the buzzer on TC001.
    pinMode(kBuzzerPin, INPUT_PULLDOWN);

    FastLED.addLeds<WS2812B, kDisplayPin, GRB>(gLeds, kLedCount);
    FastLED.setBrightness(kBrightness);
    FastLED.clear(true);

    gLeds[0] = CRGB::Red;
    FastLED.show();

    Serial.println("LED 0 should now be red");
}

void loop()
{
}