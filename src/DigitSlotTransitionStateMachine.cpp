#include "DigitSlotTransitionStateMachine.h"

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
    _stateMachine(_kTransitionTable, DigitSlotTransitionState::Idle) {
}

bool DigitSlotTransitionStateMachine::handleEvent(DigitSlotTransitionEvent event) {
   return _stateMachine.handleEvent(event);
}

DigitSlotTransitionState DigitSlotTransitionStateMachine::getCurrentState() const {
   return _stateMachine.getCurrentState();
}
