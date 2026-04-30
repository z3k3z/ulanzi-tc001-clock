#ifndef DIGIT_SLOT_STATE_MACHINE_H
#define DIGIT_SLOT_STATE_MACHINE_H

#include "TableDrivenStateMachine.h"

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

   TableDrivenStateMachine<DigitSlotState, DigitSlotEvent, (unsigned int)DigitSlotState::Count,
                           (unsigned int)DigitSlotEvent::Count>
       _stateMachine;

 public:
   DigitSlotStateMachine();

   bool           handleEvent(DigitSlotEvent event);
   DigitSlotState getCurrentState() const;
};

#endif
