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
    _iDigitProvider(iDigitProvider) {
}

void Application::initialize() {
   Serial.begin(115200);
   delay(500);

   pinMode(_kBuzzerPin, INPUT_PULLDOWN);

   _displaySurface.initialize();
   _displaySurface.show();

   Serial.println("Application initialized");
}

void Application::tick() {
}
