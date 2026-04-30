#include "DigitSlotTransitionStateMachine.h"
#include "errorh.h"

const DigitSlotTransitionState DigitSlotTransitionStateMachine::_kTransitionTable[(
    unsigned int)DigitSlotTransitionState::Count][(unsigned int)DigitSlotTransitionEvent::Count] = {
    // clang-format off
    //                  Begin                                  SweepComplete                          AcknowledgeDone
    /*          Idle */ {DigitSlotTransitionState::Erasing,    DigitSlotTransitionState::Idle,        DigitSlotTransitionState::Idle},
    /*       Erasing */ {DigitSlotTransitionState::Erasing,    DigitSlotTransitionState::Revealing,   DigitSlotTransitionState::Erasing},
    /*     Revealing */ {DigitSlotTransitionState::Revealing,  DigitSlotTransitionState::Done,        DigitSlotTransitionState::Revealing},
    /*          Done */ {DigitSlotTransitionState::Erasing,    DigitSlotTransitionState::Done,        DigitSlotTransitionState::Idle},
    // clang-format on
};

DigitSlotTransitionStateMachine::DigitSlotTransitionStateMachine() :
    _currentState(DigitSlotTransitionState::Idle) {
}

bool DigitSlotTransitionStateMachine::handleEvent(DigitSlotTransitionEvent event) {
   EHInitialize;
   unsigned int uiState = (unsigned int)_currentState;
   unsigned int uiEvent = (unsigned int)event;

   EHRaiseErrorWhen((uiState >= (unsigned int)DigitSlotTransitionState::Count) ||
                        (uiEvent >= (unsigned int)DigitSlotTransitionEvent::Count),
                    EH_PACK_INT16_TO_LONG(uiState, uiEvent));
   _currentState = _kTransitionTable[uiState][uiEvent];

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

DigitSlotTransitionState DigitSlotTransitionStateMachine::getCurrentState() const {
   return _currentState;
}
