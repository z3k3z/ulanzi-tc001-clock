#include "SerialTimeSyncProvider.h"
#include "errorh.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

SerialTimeSyncProvider::SerialTimeSyncProvider() :
    _uiLineLength(0), _fHasSynced(false), _lastSyncEpoch(0), _ulLastSyncMillis(0) {

   _szLineBuffer[0] = '\0';
}

bool SerialTimeSyncProvider::initialize() {
   EHInitialize;

   _resetLineBuffer();
   _fHasSynced       = false;
   _lastSyncEpoch    = 0;
   _ulLastSyncMillis = 0;

   return EHIsSuccess;
}

bool SerialTimeSyncProvider::handleTick(bool& fTimeWasUpdated) {
   EHInitialize;
   bool fSuccess = false;

   fTimeWasUpdated = false;

   fSuccess = _processAvailableSerial(fTimeWasUpdated);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::getLastSyncEpoch(time_t& epoch) const {
   EHInitialize;

   EHRaiseErrorWhen(!_fHasSynced, 0);
   epoch = _lastSyncEpoch;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::getLastSyncMillis(unsigned long& ulMillis) const {
   EHInitialize;

   EHRaiseErrorWhen(!_fHasSynced, 0);
   ulMillis = _ulLastSyncMillis;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::_processAvailableSerial(bool& fTimeWasUpdated) {
   EHInitialize;
   bool fSuccess = false;

   fTimeWasUpdated = false;

   while (Serial.available() > 0) {
      char ch = (char)Serial.read();

      if ('\r' == ch) {
         continue;
      }

      if ('\n' == ch) {
         _szLineBuffer[_uiLineLength] = '\0';

         fSuccess = _processLine(_szLineBuffer, fTimeWasUpdated);
         EHRaiseErrorWhenNotSuccess(fSuccess, _uiLineLength);

         _resetLineBuffer();
      } else if (_uiLineLength < (_kLineBufferSize - 1)) {
         _szLineBuffer[_uiLineLength] = ch;
         _uiLineLength++;
         _szLineBuffer[_uiLineLength] = '\0';
      } else {
         _resetLineBuffer();
         EHRaiseError(0);
      }
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::_processLine(const char* pszLine, bool& fTimeWasUpdated) {
   EHInitialize;
   bool   fSuccess = false;
   time_t epoch    = 0;

   fTimeWasUpdated = false;

   fSuccess = _tryParseEpochCommand(pszLine, epoch);
   if (fSuccess) {
      fSuccess = _setSystemTime(epoch);
      EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)epoch);

      _fHasSynced       = true;
      _lastSyncEpoch    = epoch;
      _ulLastSyncMillis = millis();
      fTimeWasUpdated   = true;

      Serial.print("TIME_SYNCED:epoch=");
      Serial.print((long)epoch);
      Serial.print(",millis=");
      Serial.println(millis());
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::_tryParseEpochCommand(const char* pszLine, time_t& epoch) const {
   EHInitialize;

   EHRaiseErrorWhen(nullptr == pszLine, 0);

   // Protocol:
   //    TIME:<unix_epoch_seconds>
   //
   // Example:
   //    TIME:1777651200
   if (0 == strncmp(pszLine, "TIME:", 5)) {
      const char* pszEpoch = pszLine + 5;
      char*       pszEnd   = nullptr;
      long        lValue   = strtol(pszEpoch, &pszEnd, 10);

      EHRaiseErrorWhen(pszEpoch == pszEnd, 0);
      EHRaiseErrorWhen(('\0' != *pszEnd), 0);
      EHRaiseErrorWhen(lValue <= 0, 0);

      epoch = (time_t)lValue;
   } else {
      EHRaiseError(0);
   }

End:
   return EHIsSuccess;
}

bool SerialTimeSyncProvider::_setSystemTime(time_t epoch) {
   EHInitialize;

   struct timeval tv;
   tv.tv_sec  = epoch;
   tv.tv_usec = 0;

   int iResult = settimeofday(&tv, nullptr);
   EHRaiseErrorWhen(0 != iResult, iResult);

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

void SerialTimeSyncProvider::_resetLineBuffer() {
   _uiLineLength    = 0;
   _szLineBuffer[0] = '\0';
}
