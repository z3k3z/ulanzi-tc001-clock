#include "Application.h"
#include "FiveByEightDigitProvider.h"

static IDigitProvider* gIDIgitProvider = new FiveByEightDigitProvider();
static Application     gApplication(*gIDIgitProvider);

void setup() {
   gApplication.initialize();
}

void loop() {
   gApplication.tick();
}
