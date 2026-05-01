#ifndef SERIAL_TIME_SYNC_PROVIDER_H
#define SERIAL_TIME_SYNC_PROVIDER_H

#include <Arduino.h>
#include <time.h>

class SerialTimeSyncProvider {
 private:
   static constexpr unsigned int _kLineBufferSize = 32;

   char          _szLineBuffer[_kLineBufferSize];
   unsigned int  _uiLineLength;
   bool          _fHasSynced;
   time_t        _lastSyncEpoch;
   unsigned long _ulLastSyncMillis;

 public:
   SerialTimeSyncProvider();

   bool initialize();
   bool handleTick(bool& fTimeWasUpdated);

   bool getHasSynced() const { return _fHasSynced; }
   bool getLastSyncEpoch(time_t& epoch) const;
   bool getLastSyncMillis(unsigned long& ulMillis) const;

 private:
   bool _processAvailableSerial(bool& fTimeWasUpdated);
   bool _processLine(const char* pszLine, bool& fTimeWasUpdated);
   bool _tryParseEpochCommand(const char* pszLine, time_t& epoch) const;
   bool _setSystemTime(time_t epoch);
   void _resetLineBuffer();
};

#endif
