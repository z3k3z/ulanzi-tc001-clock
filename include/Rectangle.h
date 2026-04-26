#ifndef RECTANGLE_H
#define RECTANGLE_H

class Rectangle {
 private:
   int          _iX;
   int          _iY;
   unsigned int _uiWidth;
   unsigned int _uiHeight;

 public:
   Rectangle(int iX, int iY, unsigned int uiWidth, unsigned int uiHeight) :
       _iX(iX), _iY(iY), _uiWidth(uiWidth), _uiHeight(uiHeight) {}

   int getX() const { return _iX; }

   int getY() const { return _iY; }

   unsigned int getWidth() const { return _uiWidth; }

   unsigned int getHeight() const { return _uiHeight; }

   bool contains(int iX, int iY) const {
      return (iX >= _iX) && (iX < (_iX + (int)_uiWidth)) && (iY >= _iY) &&
             (iY < (_iY + (int)_uiHeight));
   }
};

#endif
