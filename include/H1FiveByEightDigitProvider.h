#ifndef H1FIVE_BY_EIGHT_DIGIT_PROVIDER_H
#define H1FIVE_BY_EIGHT_DIGIT_PROVIDER_H

#include "FiveByEightDigitProvider.h"

// specific override to blank digit zero on the H1 position
class H1FiveByEightDigitProvider : public FiveByEightDigitProvider {
 private:
   static constexpr unsigned int _kGlyphWidth  = 5;
   static constexpr unsigned int _kGlyphHeight = 8;

   static const uint8_t _kDigit0Rows[_kGlyphHeight];

   PixelGlyph        _digit0Glyph;
   const PixelGlyph* _pDigitGlyphs[1];

 public:
   H1FiveByEightDigitProvider();

   bool getDigitFor(unsigned int uiDigit, const PixelGlyph*& pDigitGlyph) const override;
};

#endif
