#ifndef POINT_H
#define POINT_H

class Point {
 private:
   int _iX;
   int _iY;

 public:
   Point(int iX, int iY) : _iX(iX), _iY(iY) {}
   Point() {
      _iX = 0;
      _iY = 0;
   }

   int getX() const { return _iX; }

   int getY() const { return _iY; }

   void setX(int iX) { _iX = iX; }

   void setY(int iY) { _iY = iY; }

   void set(int iX, int iY) {
      _iX = iX;
      _iY = iY;
   }
   Point operator+(const Point& other) const { return Point(_iX + other._iX, _iY + other._iY); }
};

#endif
