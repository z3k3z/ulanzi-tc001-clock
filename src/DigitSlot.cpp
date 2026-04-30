#include "DigitSlot.h"
#include "errorh.h"

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

      _digitTransitionSweep.initialize(*pPixelGlyph, _ptOrigin, DigitTransitionSweepMode::Erase);

      fSuccess = _pixelSweeper.restart();
      EHRaiseErrorWhenNotSuccess(fSuccess, uiNewValue);

      fSuccess = _stateMachine.handleEvent(DigitSlotEvent::BeginTransition);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);
      fSuccess = _transitionStateMachine.handleEvent(DigitSlotTransitionEvent::Begin);
      EHRaiseErrorWhenNotSuccess(fSuccess, 0);
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
         fSuccess = _transitionStateMachine.handleEvent(DigitSlotTransitionEvent::SweepComplete);
         EHRaiseErrorWhenNotSuccess(fSuccess, 0);
         switch (_transitionStateMachine.getCurrentState()) {
         case DigitSlotTransitionState::Done:
            _uiCurrentValue = _uiTargetValue;

            fSuccess = _stateMachine.handleEvent(DigitSlotEvent::SweepComplete);
            EHRaiseErrorWhenNotSuccess(fSuccess, (unsigned int)DigitSlotEvent::SweepComplete);
            break;
         case DigitSlotTransitionState::Revealing:
            _digitTransitionSweep.setMode(DigitTransitionSweepMode::Reveal);
            fSuccess = _pixelSweeper.restart();
            EHRaiseErrorWhenNotSuccess(fSuccess, 0);
            break;
         default:
            EHRaiseError(_transitionStateMachine.getCurrentState());
            break;
         }
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
