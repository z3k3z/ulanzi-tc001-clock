#ifndef COLOR_MANAGER_H
#define COLOR_MANAGER_H

#include <FastLED.h>

enum class ColorTheme {
   RedLed = 0,
   TransitYellowGreen,
   AgedPhosphor,
   WarmBusMarquee,
   Count,
};

typedef struct _ColorThemeDefinition {
   CRGB    activeColor;
   CRGB    inactiveColor;
   CRGB    transitionCursorColor;
   uint8_t uiGlobalBrightness;
} ColorThemeDefinition;

class ColorManager {
 private:
   static const ColorThemeDefinition _kThemeDefinitions[];

   ColorTheme _colorTheme;

   const ColorThemeDefinition& getThemeDefinition() const;

 public:
   ColorManager();

   void setTheme(ColorTheme colorTheme);

   uint8_t getGlobalBrightness() const;
   CRGB    getActiveColor() const;
   CRGB    getInactiveColor() const;
   CRGB    getTransitionCursorColor() const;
};

#endif
