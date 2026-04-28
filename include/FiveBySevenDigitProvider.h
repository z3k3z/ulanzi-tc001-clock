#ifndef FIVE_BY_SEVEN_DIGIT_PROVIDER_H
#define FIVE_BY_SEVEN_DIGIT_PROVIDER_H

#include "IDigitProvider.h"

class FiveBySevenDigitProvider : public IDigitProvider {
 private:
   static constexpr unsigned int _kGlyphWidth      = 5;
   static constexpr unsigned int _kGlyphHeight     = 7;
   static constexpr unsigned int _kGlyphDigitCount = 10;

   static const uint8_t _kDigit0Rows[_kGlyphHeight];
   static const uint8_t _kDigit1Rows[_kGlyphHeight];
   static const uint8_t _kDigit2Rows[_kGlyphHeight];
   static const uint8_t _kDigit3Rows[_kGlyphHeight];
   static const uint8_t _kDigit4Rows[_kGlyphHeight];
   static const uint8_t _kDigit5Rows[_kGlyphHeight];
   static const uint8_t _kDigit6Rows[_kGlyphHeight];
   static const uint8_t _kDigit7Rows[_kGlyphHeight];
   static const uint8_t _kDigit8Rows[_kGlyphHeight];
   static const uint8_t _kDigit9Rows[_kGlyphHeight];

   PixelGlyph _digit0Glyph;
   PixelGlyph _digit1Glyph;
   PixelGlyph _digit2Glyph;
   PixelGlyph _digit3Glyph;
   PixelGlyph _digit4Glyph;
   PixelGlyph _digit5Glyph;
   PixelGlyph _digit6Glyph;
   PixelGlyph _digit7Glyph;
   PixelGlyph _digit8Glyph;
   PixelGlyph _digit9Glyph;

   const PixelGlyph* _pDigitGlyphs[_kGlyphDigitCount];

 public:
   FiveBySevenDigitProvider();

   bool getDigitFor(unsigned int uiDigit, const PixelGlyph*& pDigitGlyph) const override;
};

#endif
