#include "Application.h"

static Application gApplication;

void setup() {
   gApplication.initialize();
}

void loop() {
   gApplication.tick();
}
