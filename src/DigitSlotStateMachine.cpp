#include "DigitSlotStateMachine.h"

const DigitSlotState DigitSlotStateMachine::_kTransitionTable[(unsigned int)DigitSlotState::Count][(
    unsigned int)DigitSlotEvent::Count] = {
    // clang-format off
    //           Event=> BeginTransition                 SweepComplete               CompleteAcknowledged
    /*          Idle */  {DigitSlotState::Transitioning, DigitSlotState::Idle,       DigitSlotState::Idle},
    /* Transitioning */  {DigitSlotState::Transitioning, DigitSlotState::Complete,   DigitSlotState::Transitioning},
    /*      Complete */  {DigitSlotState::Transitioning, DigitSlotState::Complete,   DigitSlotState::Idle},
    // clang-format on
};

DigitSlotStateMachine::DigitSlotStateMachine() :
    _stateMachine(_kTransitionTable, DigitSlotState::Idle) {
}

bool DigitSlotStateMachine::handleEvent(DigitSlotEvent event) {
   return _stateMachine.handleEvent(event);
}

DigitSlotState DigitSlotStateMachine::getCurrentState() const {
   return _stateMachine.getCurrentState();
}
