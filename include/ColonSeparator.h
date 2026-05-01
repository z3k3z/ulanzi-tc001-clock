#ifndef COLON_SEPARATOR_H
#define COLON_SEPARATOR_H

#include "DisplaySurface.h"
#include "Point.h"

class ColonSeparator {
 private:
   Point         _ptOrigin;
   unsigned int  _uiHeight;
   unsigned int  _uiBlinkIntervalMs;
   bool          _fIsDotActive;
   bool          _fHasTickContext;
   unsigned long _ulLastToggleMs;

 public:
   ColonSeparator(const Point& ptOrigin, unsigned int uiHeight, unsigned int uiBlinkIntervalMs) :
       _ptOrigin(ptOrigin),
       _uiHeight(uiHeight),
       _uiBlinkIntervalMs(uiBlinkIntervalMs),
       _fIsDotActive(true),
       _fHasTickContext(false),
       _ulLastToggleMs(0) {}

   bool initialize(DisplaySurface& displaySurface);
   bool handleTick(DisplaySurface& displaySurface, bool& fDisplayDirty);

 private:
   bool _render(DisplaySurface& displaySurface);
};

#endif
