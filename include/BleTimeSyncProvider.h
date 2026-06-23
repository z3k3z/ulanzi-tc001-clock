#ifndef BLE_TIME_SYNC_PROVIDER_H
#define BLE_TIME_SYNC_PROVIDER_H

#include <Arduino.h>
#include <NimBLEDevice.h>
#include <time.h>

class BleTimeSyncProvider {
 private:
   static constexpr const char* _kDeviceName          = "TC001 Clock";
   static constexpr const char* _kTimeSyncServiceUuid = "7d9f1000-6f8d-4b8a-9d4a-0e4f7a001001";
   static constexpr const char* _kTimeSyncCharacteristicUuid =
       "7d9f1001-6f8d-4b8a-9d4a-0e4f7a001001";

   bool          _fHasSynced;
   time_t        _pendingSyncEpoch;
   time_t        _lastSyncEpoch;
   unsigned long _ulLastSyncMillis;

   NimBLEServer*          _pServer;
   NimBLEService*         _pService;
   NimBLECharacteristic*  _pTimeCharacteristic;
   NimBLEServerCallbacks* _pServerCallbacks;

   class ServerCallbacks : public NimBLEServerCallbacks {
    public:
      void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override {
         NimBLEDevice::startAdvertising();
         Serial.println("BLE advertising restarted");
      }
   };

   class TimeSyncCharacteristicCallbacks : public NimBLECharacteristicCallbacks {
    private:
      BleTimeSyncProvider& _provider;

    public:
      TimeSyncCharacteristicCallbacks(BleTimeSyncProvider& provider) : _provider(provider) {}

      void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override {
         (void)connInfo;
         _provider.handleCharacteristicWrite(pCharacteristic);
      }
   };

   TimeSyncCharacteristicCallbacks _timeSyncCallbacks;

 public:
   BleTimeSyncProvider();

   bool initialize();
   bool handleTick(bool& fTimeWasUpdated);

   bool getHasSynced() const { return _fHasSynced; }
   bool getLastSyncEpoch(time_t& epoch) const;
   bool getLastSyncMillis(unsigned long& ulMillis) const;

   bool handleCharacteristicWrite(NimBLECharacteristic* pCharacteristic);

 private:
   bool _processLine(const char* pszLine, bool& fTimeWasUpdated);
   bool _tryParseEpochCommand(const char* pszLine, time_t& epoch) const;
   bool _setSystemTime(time_t epoch);
};

#endif
