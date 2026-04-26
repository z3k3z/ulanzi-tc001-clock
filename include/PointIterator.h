#ifndef POINT_ITERATOR_H
#define POINT_ITERATOR_H

#include "Point.h"
#include "Rectangle.h"

enum class IterationOrder {
   XMajor,
   YMajor,
};

class PointIterator {
 private:
   Rectangle      _rectangle;
   IterationOrder _iterationOrder;

   unsigned int _uiCurrentX;
   unsigned int _uiCurrentY;
   bool         _fComplete;

 public:
   PointIterator(const Rectangle& rectangle, IterationOrder iterationOrder) :
       _rectangle(rectangle),
       _iterationOrder(iterationOrder),
       _uiCurrentX(0),
       _uiCurrentY(0),
       _fComplete(false) {}

   void reset() {
      _uiCurrentX = 0;
      _uiCurrentY = 0;
      _fComplete  = false;
   }

   bool next(Point& point) {
      if (_fComplete) {
         return false;
      }

      point.set(_rectangle.getX() + (int)_uiCurrentX, _rectangle.getY() + (int)_uiCurrentY);

      if (_iterationOrder == IterationOrder::XMajor) {
         ++_uiCurrentX;

         if (_uiCurrentX >= _rectangle.getWidth()) {
            _uiCurrentX = 0;
            ++_uiCurrentY;

            if (_uiCurrentY >= _rectangle.getHeight()) {
               _fComplete = true;
            }
         }
      } else {
         ++_uiCurrentY;

         if (_uiCurrentY >= _rectangle.getHeight()) {
            _uiCurrentY = 0;
            ++_uiCurrentX;

            if (_uiCurrentX >= _rectangle.getWidth()) {
               _fComplete = true;
            }
         }
      }

      return true;
   }
};

#endif
