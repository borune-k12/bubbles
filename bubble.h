#ifndef BUBBLE_H
#define BUBBLE_H

#include <math.h>

const double BUBBLE_WEIGHT = 10.;    // масса шарика
const double BUBBLE_RADIUS = 25.;    // радиус шарика

/**
 * @brief The Bubble struct структура, описывающая шарик
 */
struct Bubble {
    double x;    // координата по x
    double y;    // координата по y
    double vx;   // скорость вдоль оси x
    double vy;   // скорость вдоль оси y

    Bubble(double x, double y):x(x),y(y),vx(0),vy(0){}
    // переопределяем оператор, чтоб понимать, когда кликнули на тот же самый шарик и удалить его
    bool operator==(const Bubble& other){return fabs(other.x-x)<BUBBLE_RADIUS && fabs(other.y-y)<BUBBLE_RADIUS;}
};

#endif // BUBBLE_H
