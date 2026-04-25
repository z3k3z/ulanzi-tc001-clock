#include "ColorManager.h"

// clang-format off
const ColorThemeDefinition ColorManager::_kThemeDefinitions[] = {
    // activeColor          inactiveColor    cursorColor  brightness
    {CRGB(255, 0, 0),       CRGB(8, 0, 0),    CRGB::White, 50}, // RedLed
    {CRGB(180, 255, 40),    CRGB(10, 14, 2),  CRGB::White, 50}, // TransitYellowGreen
    {CRGB(140, 220, 35),    CRGB(8, 12, 2),   CRGB::White, 50}, // AgedPhosphor
    {CRGB(200, 180, 20),    CRGB(12, 10, 1),  CRGB::White, 50}, // WarmBusMarquee
};
// clang-format on

ColorManager::ColorManager() : _colorTheme(ColorTheme::RedLed) {
}

void ColorManager::setTheme(ColorTheme colorTheme) {
   _colorTheme = colorTheme;
}

const ColorThemeDefinition& ColorManager::getThemeDefinition() const {
   unsigned int uiThemeIndex = (unsigned int)_colorTheme;

   if (uiThemeIndex >= (unsigned int)ColorTheme::Count) {
      uiThemeIndex = (unsigned int)ColorTheme::RedLed;
   }

   return _kThemeDefinitions[uiThemeIndex];
}

uint8_t ColorManager::getGlobalBrightness() const {
   return getThemeDefinition().uiGlobalBrightness;
}

CRGB ColorManager::getActiveColor() const {
   return getThemeDefinition().activeColor;
}

CRGB ColorManager::getInactiveColor() const {
   return getThemeDefinition().inactiveColor;
}

CRGB ColorManager::getTransitionCursorColor() const {
   return getThemeDefinition().transitionCursorColor;
}
