#include "Application.h"
#include "FiveBySevenDigitProvider.h"

static IDigitProvider* gIDIgitProvider = new FiveBySevenDigitProvider();
static Application     gApplication(*gIDIgitProvider);

void setup() {
   gApplication.initialize();
}

void loop() {
   gApplication.tick();
}
