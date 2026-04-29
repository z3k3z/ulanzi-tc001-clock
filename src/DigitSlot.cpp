#include "DigitSlot.h"
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
      EHEmitMsgDebug
   }
   return EHIsSuccess;
}

bool DigitSlot::initialize(unsigned int uiInitialValue) {
   EHInitialize;
   bool              fSuccess    = false;
   const PixelGlyph* pPixelGlyph = nullptr;

   fSuccess = _iDigitProvider.getDigitFor(uiInitialValue, pPixelGlyph);
   EHRaiseErrorWhenNotSuccess(fSuccess, uiInitialValue);
   EHRaiseErrorWhen(nullptr == pPixelGlyph, uiInitialValue);

   fSuccess = pPixelGlyph->draw(_displaySurface, _ptOrigin);
   EHRaiseErrorWhenNotSuccess(fSuccess, uiInitialValue);

   _uiCurrentValue = uiInitialValue;
   _uiTargetValue  = uiInitialValue;

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool DigitSlot::beginTransitionTo(unsigned int uiNewValue) {
   EHInitialize;
   bool              fSuccess    = false;
   const PixelGlyph* pPixelGlyph = nullptr;

   if (uiNewValue != _uiCurrentValue) {
      fSuccess = _iDigitProvider.getDigitFor(uiNewValue, pPixelGlyph);
      EHRaiseErrorWhenNotSuccess(fSuccess, uiNewValue);
      EHRaiseErrorWhen(nullptr == pPixelGlyph, uiNewValue);

      _uiTargetValue = uiNewValue;

      _digitTransitionSweep.initialize(*pPixelGlyph, _ptOrigin);

      fSuccess = _pixelSweeper.restart();
      EHRaiseErrorWhenNotSuccess(fSuccess, uiNewValue);

      fSuccess = _stateMachine.handleEvent(DigitSlotEvent::BeginTransition);
      EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)DigitSlotEvent::BeginTransition);
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}

bool DigitSlot::handleTick() {
   EHInitialize;
   bool fSuccess = false;

   switch (_stateMachine.getCurrentState()) {
   case DigitSlotState::Transitioning:
      fSuccess = _pixelSweeper.handleTick();
      EHRaiseErrorWhenNotSuccess(fSuccess, _uiTargetValue);

      if (_pixelSweeper.getIsDone()) {
         _uiCurrentValue = _uiTargetValue;

         fSuccess = _stateMachine.handleEvent(DigitSlotEvent::SweepComplete);
         EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)DigitSlotEvent::SweepComplete);
      }
      break;

   case DigitSlotState::Complete:
      fSuccess = _stateMachine.handleEvent(DigitSlotEvent::CompleteAcknowledged);
      EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)DigitSlotEvent::CompleteAcknowledged);
      break;

   case DigitSlotState::Idle:
   case DigitSlotState::Count:
   default:
      break;
   }

End:
   if (EHErrorRaised) {
      EHEmitMsgDebug;
   }
   return EHIsSuccess;
}
