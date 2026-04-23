#ifndef COORDINATE_MAPPER_H
#define COORDINATE_MAPPER_H

class CoordinateMapper {

private:
    unsigned int _uiWidth;
    unsigned int _uiHeight;

public:
    CoordinateMapper(
        unsigned int uiWidth,
        unsigned int uiHeight) {
            _uiWidth = uiWidth;
            _uiHeight = uiHeight;
        };

    // obtain the LED index from the matrix coordinates
    bool indexFromCoordinates( int iX, int iY, unsigned int& uiIndex);

};
#endif
