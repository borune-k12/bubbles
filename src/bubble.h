#ifndef BUBBLE_H
#define BUBBLE_H

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

    // начальные скорости шарика делаем равными 0
    Bubble(double x, double y):x(x),y(y),vx(0),vy(0){}    
};

#endif // BUBBLE_H
