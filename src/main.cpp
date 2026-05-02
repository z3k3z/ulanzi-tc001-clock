#include "Application.h"
#include "FiveByEightDigitProvider.h"
#include "H1FiveByEightDigitProvider.h"

static IDigitProvider* gIDigitProvider    = new FiveByEightDigitProvider();
static IDigitProvider* gIDigitProviderAlt = new H1FiveByEightDigitProvider();
static Application     gApplication(*gIDigitProvider, *gIDigitProviderAlt);

void setup() {
   gApplication.initialize();
}

void loop() {
   gApplication.tick();
}
