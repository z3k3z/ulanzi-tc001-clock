#include "FiveBySevenDigitProvider.h"
#include "errorh.h"

// clang-format off
const uint8_t FiveBySevenDigitProvider::_kDigit0Rows[_kGlyphHeight] = {
      0b01110,
      0b10001,
      0b10011,
      0b10101,
      0b11001,
      0b10001,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit1Rows[_kGlyphHeight] = {
      0b00100,
      0b01100,
      0b00100,
      0b00100,
      0b00100,
      0b00100,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit2Rows[_kGlyphHeight] = {
      0b01110,
      0b10001,
      0b00001,
      0b00010,
      0b00100,
      0b01000,
      0b11111,
};
const uint8_t FiveBySevenDigitProvider::_kDigit3Rows[_kGlyphHeight] = {
      0b01110,
      0b10001,
      0b00001,
      0b00110,
      0b00001,
      0b10001,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit4Rows[_kGlyphHeight] = {
      0b00010,
      0b00110,
      0b01010,
      0b10010,
      0b11111,
      0b00010,
      0b00010,
};
const uint8_t FiveBySevenDigitProvider::_kDigit5Rows[_kGlyphHeight] = {
      0b11111,
      0b10000,
      0b11110,
      0b00001,
      0b00001,
      0b10001,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit6Rows[_kGlyphHeight] = {
      0b00110,
      0b01000,
      0b10000,
      0b11110,
      0b10001,
      0b10001,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit7Rows[_kGlyphHeight] = {
      0b11111,
      0b00001,
      0b00010,
      0b00100,
      0b01000,
      0b01000,
      0b01000,
};
const uint8_t FiveBySevenDigitProvider::_kDigit8Rows[_kGlyphHeight] = {
      0b01110,
      0b10001,
      0b10001,
      0b01110,
      0b10001,
      0b10001,
      0b01110,
};
const uint8_t FiveBySevenDigitProvider::_kDigit9Rows[_kGlyphHeight] = {
      0b01110,
      0b10001,
      0b10001,
      0b01111,
      0b00001,
      0b00010,
      0b01100,
};
// clang-format on

FiveBySevenDigitProvider::FiveBySevenDigitProvider() :
    _digit0Glyph(_kDigit0Rows, _kGlyphWidth, _kGlyphHeight),
    _digit1Glyph(_kDigit1Rows, _kGlyphWidth, _kGlyphHeight),
    _digit2Glyph(_kDigit2Rows, _kGlyphWidth, _kGlyphHeight),
    _digit3Glyph(_kDigit3Rows, _kGlyphWidth, _kGlyphHeight),
    _digit4Glyph(_kDigit4Rows, _kGlyphWidth, _kGlyphHeight),
    _digit5Glyph(_kDigit5Rows, _kGlyphWidth, _kGlyphHeight),
    _digit6Glyph(_kDigit6Rows, _kGlyphWidth, _kGlyphHeight),
    _digit7Glyph(_kDigit7Rows, _kGlyphWidth, _kGlyphHeight),
    _digit8Glyph(_kDigit8Rows, _kGlyphWidth, _kGlyphHeight),
    _digit9Glyph(_kDigit9Rows, _kGlyphWidth, _kGlyphHeight),
    _pDigitGlyphs{
        &_digit0Glyph, &_digit1Glyph, &_digit2Glyph, &_digit3Glyph, &_digit4Glyph,
        &_digit5Glyph, &_digit6Glyph, &_digit7Glyph, &_digit8Glyph, &_digit9Glyph,
    } {
}

bool FiveBySevenDigitProvider::getDigitFor(unsigned int       uiDigit,
                                           const PixelGlyph*& pDigitGlyph) const {
   EHInitialize;

   EHRaiseErrorWhen(uiDigit >= _kGlyphDigitCount, uiDigit);
   pDigitGlyph = _pDigitGlyphs[uiDigit];

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
