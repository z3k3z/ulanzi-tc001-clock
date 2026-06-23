#include "BleTimeSyncProvider.h"
#include "errorh.h"
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

BleTimeSyncProvider::BleTimeSyncProvider() :
    _fHasSynced(false),
    _pendingSyncEpoch(0),
    _ulLastSyncMillis(0),
    _pServer(nullptr),
    _pService(nullptr),
    _pTimeCharacteristic(nullptr),
    _timeSyncCallbacks(*this),
    _pServerCallbacks(new ServerCallbacks()) {
}

bool BleTimeSyncProvider::initialize() {
   EHInitialize;

   _fHasSynced       = false;
   _pendingSyncEpoch = 0;
   _ulLastSyncMillis = 0;

   NimBLEDevice::init(_kDeviceName);

   _pServer = NimBLEDevice::createServer();
   EHRaiseErrorWhen(nullptr == _pServer, 0);

   _pService = _pServer->createService(_kTimeSyncServiceUuid);
   EHRaiseErrorWhen(nullptr == _pService, 0);

   _pTimeCharacteristic =
       _pService->createCharacteristic(_kTimeSyncCharacteristicUuid, NIMBLE_PROPERTY::WRITE);
   EHRaiseErrorWhen(nullptr == _pTimeCharacteristic, 0);

   _pTimeCharacteristic->setCallbacks(&_timeSyncCallbacks);

   _pServer->setCallbacks(_pServerCallbacks);

   _pService->start();
   Serial.println("BLE time sync advertising started");

   {
      NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
      EHRaiseErrorWhen(nullptr == pAdvertising, 0);

      NimBLEAdvertisementData advData;
      advData.setName(_kDeviceName);
      advData.setFlags(0x06); // General discoverable + BR/EDR not supported
      advData.addServiceUUID(_kTimeSyncServiceUuid);

      NimBLEAdvertisementData scanData;
      scanData.setName(_kDeviceName);

      pAdvertising->setAdvertisementData(advData);
      pAdvertising->setScanResponseData(scanData);

      pAdvertising->start();

      Serial.print("BLE advertising active as ");
      Serial.println(_kDeviceName);
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool BleTimeSyncProvider::handleTick(bool& fTimeWasUpdated) {
   EHInitialize;

   fTimeWasUpdated = false;
   // do we have a pending time change?
   if (0 != _pendingSyncEpoch) {
      time_t epoch      = _pendingSyncEpoch;
      _pendingSyncEpoch = 0;

      bool fSuccess = _setSystemTime(epoch);
      EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)epoch);

      _fHasSynced       = true;
      _lastSyncEpoch    = epoch;
      _ulLastSyncMillis = millis();
      fTimeWasUpdated   = true;

      Serial.print("BLE_TIME_SYNCED:epoch=");
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

bool BleTimeSyncProvider::getLastSyncEpoch(time_t& epoch) const {
   EHInitialize;

   EHRaiseErrorWhen(!_fHasSynced, 0);
   epoch = _lastSyncEpoch;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool BleTimeSyncProvider::getLastSyncMillis(unsigned long& ulMillis) const {
   EHInitialize;

   EHRaiseErrorWhen(!_fHasSynced, 0);
   ulMillis = _ulLastSyncMillis;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool BleTimeSyncProvider::handleCharacteristicWrite(NimBLECharacteristic* pCharacteristic) {
   EHInitialize;
   bool fSuccess        = false;
   bool fTimeWasUpdated = false;

   EHRaiseErrorWhen(nullptr == pCharacteristic, 0);

   {
      std::string value = pCharacteristic->getValue();
      EHRaiseErrorWhen(value.empty(), 0);

      // The expected BLE payload is the same as serial:
      //    TIME:<unix_epoch_seconds>
      //
      // Copy into a small local C string so the parser stays close to SerialTimeSyncProvider.
      char szLine[32];
      EHRaiseErrorWhen(value.length() >= sizeof(szLine), value.length());

      strncpy(szLine, value.c_str(), sizeof(szLine));
      szLine[sizeof(szLine) - 1] = '\0';

      fSuccess = _processLine(szLine, fTimeWasUpdated);
      EHRaiseErrorWhenNotSuccess(fSuccess, value.length());
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool BleTimeSyncProvider::_processLine(const char* pszLine, bool& fTimeWasUpdated) {
   EHInitialize;
   bool   fSuccess = false;
   time_t epoch    = 0;

   fTimeWasUpdated = false;

   fSuccess = _tryParseEpochCommand(pszLine, epoch);
   EHRaiseErrorWhenNotSuccess(fSuccess, 0);
   _pendingSyncEpoch = epoch;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool BleTimeSyncProvider::_tryParseEpochCommand(const char* pszLine, time_t& epoch) const {
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

bool BleTimeSyncProvider::_setSystemTime(time_t epoch) {
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
