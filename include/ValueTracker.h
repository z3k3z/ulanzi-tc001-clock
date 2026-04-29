#ifndef VALUE_TRACKER_H
#define VALUE_TRACKER_H

class ValueTracker {
 private:
   static constexpr unsigned int _kMaxDigits = 4;
   bool                          _fHasPreviousValue;
   int                           _iPreviousValue;
   int                           _iCurrentValue;
   // holds info only after an update causes a change
   char _szPrevious[_kMaxDigits + 1];
   char _szCurrent[_kMaxDigits + 1];

 public:
   ValueTracker() : _fHasPreviousValue(false), _iPreviousValue(0), _iCurrentValue(0) {}

   bool setInitialValue(int iValue) {
      setCurrentValue(iValue);
      _fHasPreviousValue = false;
      _szCurrent[0]      = '\0';
      _szPrevious[0]     = '\0';
      return true;
   }

   bool updateValue(int iNewValue, bool& fHasChanged);

   bool queryDigitNewValue(unsigned int uiDigitOffsetFromRight, int& iDigitValue,
                           bool& fHasChanged) const;

 private:
   void setCurrentValue(int iValue) { _iCurrentValue = iValue % 10000; } // limit to 4 digits

   void setPreviousValue(int iValue) {
      _iPreviousValue    = iValue;
      _fHasPreviousValue = true;
   }
};

#endif
