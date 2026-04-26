#ifndef POINT_PATH_H
#define POINT_PATH_H

#include "Point.h"

class PointPath {
 private:
   const Point* _pPoints;
   unsigned int _uiCount;

 public:
   PointPath(const Point* pPoints, unsigned int uiCount) : _pPoints(pPoints), _uiCount(uiCount) {}

   bool getPointAt(unsigned int uiIndex, Point& point) const {
      if (uiIndex >= _uiCount) {
         return false;
      }

      point = _pPoints[uiIndex];
      return true;
   }

   unsigned int getCount() const { return _uiCount; }
};

#endif
