#ifndef DIGIT_SLOT_H
#define DIGIT_SLOT_H

#include "DigitSlotTransitionStateMachine.h"
#include "DigitTransitionSweep.h"
#include "DisplaySurface.h"
#include "IDigitProvider.h"
#include "PixelSweeper.h"
#include "Point.h"
#include "PointPath.h"

enum class DigitSlotState {
   Idle = 0,
   Transitioning,
   Complete,
   Count,
};

enum class DigitSlotEvent {
   BeginTransition = 0,
   SweepComplete,
   CompleteAcknowledged,
   Count,
};

class DigitSlotStateMachine {
 private:
   static const DigitSlotState _kTransitionTable[(unsigned int)DigitSlotState::Count]
                                                [(unsigned int)DigitSlotEvent::Count];

   DigitSlotState _currentState;

 public:
   DigitSlotStateMachine() : _currentState(DigitSlotState::Idle) {}

   bool           handleEvent(DigitSlotEvent event);
   DigitSlotState getCurrentState() const { return _currentState; }
};

class DigitSlot {
 private:
   Point                           _ptOrigin;
   unsigned int                    _uiCurrentValue;
   unsigned int                    _uiTargetValue;
   DisplaySurface&                 _displaySurface;
   const IDigitProvider&           _iDigitProvider;
   DigitTransitionSweep            _digitTransitionSweep;
   PixelSweeper                    _pixelSweeper;
   DigitSlotStateMachine           _stateMachine;
   DigitSlotTransitionStateMachine _transitionStateMachine;

 public:
   DigitSlot(DisplaySurface& displaySurface, const IDigitProvider& iDigitProvider,
             const PointPath& pointPath, unsigned int uiSweepRateMs, const Point& ptOrigin,
             const PixelGlyph& initialGlyph) :
       _ptOrigin(ptOrigin),
       _uiCurrentValue(0),
       _uiTargetValue(0),
       _displaySurface(displaySurface),
       _iDigitProvider(iDigitProvider),
       _digitTransitionSweep(displaySurface, initialGlyph, ptOrigin),
       _pixelSweeper(pointPath, uiSweepRateMs, _digitTransitionSweep),
       _stateMachine() {}

   bool initialize(unsigned int uiInitialValue);
   bool beginTransitionTo(unsigned int uiNewValue);
   bool handleTick();

   DigitSlotState getCurrentState() const { return _stateMachine.getCurrentState(); }

   unsigned int getCurrentValue() const { return _uiCurrentValue; }
   unsigned int getTargetValue() const { return _uiTargetValue; }
};

#endif
