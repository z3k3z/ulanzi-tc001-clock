#include "ValueTracker.h"
#include "errorh.h"

bool ValueTracker::updateValue(int iNewValue, bool& fHasChanged) {
   EHInitialize;

   setPreviousValue(_iCurrentValue);
   setCurrentValue(iNewValue);

   // determine if we have changed in value
   fHasChanged = (_iCurrentValue != _iPreviousValue);
   if (fHasChanged) {
      // cache the updated values as text to be used later
      snprintf(_szCurrent, _kMaxDigits + 1, "%0*d", _kMaxDigits, _iCurrentValue);
      snprintf(_szPrevious, _kMaxDigits + 1, "%0*d", _kMaxDigits, _iPreviousValue);
   }

   return EHIsSuccess;
}

bool ValueTracker::queryDigitNewValue(unsigned int uiDigitOffsetFromRight, int& iDigitValue,
                                      bool& fHasChanged) const {
   EHInitialize;
   EHRaiseErrorWhen(!_fHasPreviousValue, 0);
   {
      int iDigitPos = (int)(_kMaxDigits - 1) - (int)uiDigitOffsetFromRight;

      EHRaiseErrorWhen((0 > iDigitPos), uiDigitOffsetFromRight);
      EHRaiseErrorWhen((unsigned int)iDigitPos >= _kMaxDigits, uiDigitOffsetFromRight);
      {
         char cNew = _szCurrent[iDigitPos];
         char cOld = _szPrevious[iDigitPos];

         if (cNew != cOld) {
            fHasChanged = true;
            iDigitValue = cNew - '0';
         } else {
            fHasChanged = false;
         }
      }
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
