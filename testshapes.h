#ifndef TESTSHAPES_H
#define TESTSHAPES_H

/**
 * координаты правильных фигур для тестирования
 */

#include "point.h"

namespace TestShapes {

    // шестиугольник
    const std::vector<Point> hexagon{
                Point(-0.75/2,-sqrt(3)/4),
                Point(0.25/2,-sqrt(3)/4),
                Point(0.75/2, 0),
                Point(0.25/2,sqrt(3)/4),
                Point(-0.75/2,sqrt(3)/4),
                Point(-1.25/2,0)};

    // квадрат
    const std::vector<Point> square{
                Point(-0.5,-0.5),
                Point(-0.5,0.5),
                Point(0.5,0.5),
                Point(0.5, -0.5)};

    // треугольник
    const std::vector<Point> triangle{
                Point(-0.5,0),
                Point(0.25,sqrt(3)/4),
                Point(0.25,-sqrt(3)/4)};

    // две точки на расстоянии 1
    const std::vector<Point> pair{
                Point(-0.5,0.5),
                Point(0.5,0.5)};
}
#endif // TESTSHAPES_H
