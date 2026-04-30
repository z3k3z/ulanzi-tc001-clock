#include "DigitSlotStateMachine.h"
#include "errorh.h"

const DigitSlotState DigitSlotStateMachine::_kTransitionTable[(unsigned int)DigitSlotState::Count][(
    unsigned int)DigitSlotEvent::Count] = {
    // clang-format off
    //           Event=> BeginTransition                 SweepComplete               CompleteAcknowledged
    /*          Idle */  {DigitSlotState::Transitioning, DigitSlotState::Idle,       DigitSlotState::Idle},
    /* Transitioning */  {DigitSlotState::Transitioning, DigitSlotState::Complete,   DigitSlotState::Transitioning},
    /*      Complete */  {DigitSlotState::Transitioning, DigitSlotState::Complete,   DigitSlotState::Idle},
    // clang-format on
};

DigitSlotStateMachine::DigitSlotStateMachine() : _currentState(DigitSlotState::Idle) {
}

bool DigitSlotStateMachine::handleEvent(DigitSlotEvent event) {
   EHInitialize;
   unsigned int uiState = (unsigned int)_currentState;
   unsigned int uiEvent = (unsigned int)event;

   EHRaiseErrorWhen(uiState >= (unsigned int)DigitSlotState::Count ||
                        uiEvent >= (unsigned int)DigitSlotEvent::Count,
                    EH_PACK_INT16_TO_LONG(uiState, uiEvent));
   _currentState = _kTransitionTable[uiState][uiEvent];

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

DigitSlotState DigitSlotStateMachine::getCurrentState() const {
   return _currentState;
}
