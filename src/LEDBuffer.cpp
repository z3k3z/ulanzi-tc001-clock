#include "LEDBuffer.h"
#include "errorh.h"

bool LEDBuffer::getLEDAt(unsigned int uiIndex, CRGB*& pLED) {
   EHInitialize;

   // validate that we are within buffer range
   EHRaiseErrorWhen((uiIndex >= LEDBuffer::_kuiCount),
                    EH_PACK_INT16_TO_LONG(uiIndex, LEDBuffer::_kuiCount));
   // access the array
   pLED = &_rgbLEDs[uiIndex];

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
