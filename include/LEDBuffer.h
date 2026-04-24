#ifndef LEDBUFFER_H
#define LEDBUFFER_H
#include <FastLED.h>

class LEDBuffer {
 private:
   static const unsigned int _kuiCount = 256;
   CRGB                      _rgbLEDs[_kuiCount];

 public:
   // provide access to the LED at index
   bool getLEDAt(unsigned int uiIndex, CRGB*& pLED);
   // return the number of LEDS in the buffer
   unsigned int getCount() const { return LEDBuffer::_kuiCount; }
   // provide r/w access to the buffer (not a fan of this)
   CRGB* getBuffer() { return _rgbLEDs; }
};

#endif
