#include "H1FiveByEightDigitProvider.h"
#include "errorh.h"

// clang-format off
const uint8_t H1FiveByEightDigitProvider::_kDigit0Rows[_kGlyphHeight] = {
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
      0b00000,
};
// clang-format on

H1FiveByEightDigitProvider::H1FiveByEightDigitProvider() :
    _digit0Glyph(_kDigit0Rows, _kGlyphWidth, _kGlyphHeight), _pDigitGlyphs{&_digit0Glyph} {
}

bool H1FiveByEightDigitProvider::getDigitFor(unsigned int       uiDigit,
                                             const PixelGlyph*& pDigitGlyph) const {
   EHInitialize;

   if (0 == uiDigit) {
      pDigitGlyph = _pDigitGlyphs[0];
   } else {
      bool fSuccess = FiveByEightDigitProvider::getDigitFor(uiDigit, pDigitGlyph);
      EHRaiseErrorWhenNotSuccess(fSuccess, uiDigit);
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
