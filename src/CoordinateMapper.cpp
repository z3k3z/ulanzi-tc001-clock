#include "CoordinateMapper.h"
#include "errorh.h"

// Calculates the LED index for the coordinate provided.
// Assumes:
//      The (0,0) origin is upper left. X increases to the right. Y increases downward.
bool CoordinateMapper::indexFromCoordinates(int iX, int iY, unsigned int& uiIndex) {
   EHInitialize;
   unsigned int uiMappedIndex = 0;

   // verify that we are within bounds
   EHRaiseErrorWhen(iX < 0, iX);
   EHRaiseErrorWhen(iY < 0, iY);
   EHRaiseErrorWhen((unsigned int)iX >= _uiWidth, iX);
   EHRaiseErrorWhen((unsigned int)iY >= _uiHeight, iY);

   // calculate the index
   if (iY % 2) {
      uiMappedIndex = (unsigned int)iY * _uiWidth + (_uiWidth - 1 - (unsigned int)iX);
   } else {
      uiMappedIndex = (unsigned int)iY * _uiWidth + (unsigned int)iX;
   }

End:
   if (EHIsSuccess) {
      uiIndex = uiMappedIndex;
   }
   return EHIsSuccess;
}
