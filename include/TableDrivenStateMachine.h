#ifndef TABLE_DRIVEN_STATE_MACHINE_H
#define TABLE_DRIVEN_STATE_MACHINE_H

template <typename TState, typename TEvent, unsigned int uiStateCount, unsigned int uiEventCount>
class TableDrivenStateMachine {
 private:
   const TState (&_transitionTable)[uiStateCount][uiEventCount];
   TState _currentState;

 public:
   TableDrivenStateMachine(const TState (&transitionTable)[uiStateCount][uiEventCount],
                           TState initialState) :
       _transitionTable(transitionTable),
       _currentState(initialState) {
   }

   bool handleEvent(TEvent event) {
      unsigned int uiState = (unsigned int)_currentState;
      unsigned int uiEvent = (unsigned int)event;

      if ((uiState >= uiStateCount) || (uiEvent >= uiEventCount)) {
         return false;
      }

      _currentState = _transitionTable[uiState][uiEvent];
      return true;
   }

   TState getCurrentState() const {
      return _currentState;
   }
};

#endif
