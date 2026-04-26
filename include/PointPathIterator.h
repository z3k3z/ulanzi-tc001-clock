#ifndef POINT_PATH_ITERATOR_H
#define POINT_PATH_ITERATOR_H

#include "Point.h"
#include "PointPath.h"

class PointPathIterator {
 private:
   const PointPath& _pointPath;
   unsigned int     _uiCurrentIndex;

 public:
   PointPathIterator(const PointPath& pointPath) : _pointPath(pointPath), _uiCurrentIndex(0) {}

   void reset() { _uiCurrentIndex = 0; }

   bool next(Point& point) {
      bool fSuccess = _pointPath.getPointAt(_uiCurrentIndex, point);

      if (fSuccess) {
         ++_uiCurrentIndex;
      }

      return fSuccess;
   }

   bool getIsDone() const { return _uiCurrentIndex >= _pointPath.getCount(); }
};

#endif
