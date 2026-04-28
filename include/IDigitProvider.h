#ifndef IDIGIT_PROVIDER_H
#define IDIGIT_PROVIDER_H

#include "PixelGlyph.h"

class IDigitProvider {
 public:
   virtual ~IDigitProvider() {}

   virtual bool getDigitFor(unsigned int uiDigit, const PixelGlyph*& pDigitGlyph) const = 0;
};

#endif
