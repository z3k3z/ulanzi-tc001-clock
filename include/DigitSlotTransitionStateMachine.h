#ifndef DIGIT_SLOT_TRANSITION_STATE_MACHINE_H
#define DIGIT_SLOT_TRANSITION_STATE_MACHINE_H

#include "TableDrivenStateMachine.h"

enum class DigitSlotTransitionState {
   Idle = 0,
   Erasing,
   Revealing,
   Done,
   Count,
};

enum class DigitSlotTransitionEvent {
   Begin = 0,
   SweepComplete,
   AcknowledgeDone,
   Count,
};

class DigitSlotTransitionStateMachine {
 private:
   static const DigitSlotTransitionState _kTransitionTable[(
       unsigned int)DigitSlotTransitionState::Count][(unsigned int)DigitSlotTransitionEvent::Count];

   TableDrivenStateMachine<DigitSlotTransitionState, DigitSlotTransitionEvent,
                           (unsigned int)DigitSlotTransitionState::Count,
                           (unsigned int)DigitSlotTransitionEvent::Count>
       _stateMachine;

 public:
   DigitSlotTransitionStateMachine();

   bool                     handleEvent(DigitSlotTransitionEvent event);
   DigitSlotTransitionState getCurrentState() const;
};

#endif
