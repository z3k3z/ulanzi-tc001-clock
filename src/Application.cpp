#include "Application.h"
#include "errorh.h"

// clang-format off
const uint8_t Application::_kDigit0Rows[7] = {
      0b01110,
      0b10001,
      0b10011,
      0b10101,
      0b11001,
      0b10001,
      0b01110,
};
const uint8_t Application::_kDigit1Rows[7] = {
      0b00100,
      0b01100,
      0b00100,
      0b00100,
      0b00100,
      0b00100,
      0b01110,
};
const Application::DigitDescriptor Application::_kDigitDescriptors[4] = {
    {Point(1, 0), ColorTheme::RedLed},
    {Point(8, 0), ColorTheme::TransitYellowGreen},
    {Point(15, 0), ColorTheme::AgedPhosphor},
    {Point(22, 0), ColorTheme::WarmBusMarquee},
};
// clang-format on

Application::Application() :
    _coordinateMapper(_kMatrixWidth, _kMatrixHeight),
    _ledBuffer(),
    _displaySurface(_coordinateMapper, _ledBuffer),
    _digit0Glyph(_kDigit0Rows, _kGlyphWidth, _kGlyphHeight),
    _digit1Glyph(_kDigit1Rows, _kGlyphWidth, _kGlyphHeight),
    _colorManager(),
    _pixelSweeper(nullptr),
    _currentDigit(0) {
}

void Application::initialize() {
   Serial.begin(115200);
   delay(500);

   pinMode(_kBuzzerPin, INPUT_PULLDOWN);

   _displaySurface.initialize();

   if (!renderThemeZeros()) {
      Serial.println("Render failure");
   }

   _displaySurface.show();

   Serial.println("Application initialized");
}

void Application::tick() {
   EHInitialize;
   bool fSuccess = false;

   // if we have an active pixel sweeper, then tick() it
   if (nullptr != _pixelSweeper) {
      fSuccess = _pixelSweeper->handleTick();
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);

      // if the sweeper is done, dispose of it
      if (_pixelSweeper->getIsDone()) {
         delete (_pixelSweeper);
         _pixelSweeper = nullptr;
         // stage the index  of the next one
         _currentDigit = ++_currentDigit % 4;
      }
   } else {
      // we don't have an active pixel sweeper.  Let's create one.
      const DigitDescriptor& desc = _kDigitDescriptors[_currentDigit];
      Rectangle rect(desc.pointOrigin.getX(), desc.pointOrigin.getY(), _kGlyphWidth, _kGlyphHeight);
      _colorManager.setTheme(_kDigitDescriptors->colorTheme);
      _pixelSweeper = new PixelSweeper(rect, 20, _displaySurface, _colorManager);
   }
   _displaySurface.show();

End:
   if (EHErrorRaised) {
      EHEmitMsg;
   }
}

bool Application::renderThemeZeros() {
   EHInitialize;
   bool fSuccess = false;

   _displaySurface.clear();

   for (const DigitDescriptor& digitDescriptor : _kDigitDescriptors) {
      _colorManager.setTheme(digitDescriptor.colorTheme);

      fSuccess = _digit0Glyph.draw(_displaySurface, digitDescriptor.pointOrigin.getX(),
                                   digitDescriptor.pointOrigin.getY(), _colorManager);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);
   }

End:
   return EHIsSuccess;
}
