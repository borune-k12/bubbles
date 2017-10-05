#ifndef COORDSCALCULATOR_H
#define COORDSCALCULATOR_H

#include <memory>
#include <vector>
#include <mutex>
#include <math.h>

class Locker;

#define DELAY 10         // задержка между тактами
#define WEIGHT 10        // масса шарика
#define BUBBLE_RADIUS 25 // радиус шарика
#define PX2LENGHT 100   // коэффициент перевода пикселей в единицы длины

/** структура для хранения данных о шарике */
struct Bubble {
    double x;    // координата по x
    double y;    // координата по y
    double vx;   // скорость вдоль оси x
    double vy;   // скорость вдоль оси y

    Bubble(double x, double y):x(x),y(y),vx(0),vy(0){}
    bool operator==(const Bubble& other){return fabs(other.x-x)<BUBBLE_RADIUS && fabs(other.y-y)<BUBBLE_RADIUS;}
};

/** Класс для расчета координат шариков */
class CoordsCalculator
{
private:
    std::vector<Bubble> bubbles_;
    bool started_;
    std::mutex mutex_;
    size_t selectedIndex_;
    std::shared_ptr<Locker> locker_;

    void _calculateCoords();

public:
    explicit CoordsCalculator(std::shared_ptr<Locker> locker);
    virtual ~CoordsCalculator();

    void start();
    void stop();

    void selectBubble(size_t index);
    void deselectBubble(bool moved, std::pair<double,double> dst = std::make_pair<double,double>(-1,-1));

    void removeAllBubbles();
    void addBubble(const Bubble& bubble);

    size_t getBubblesCount() const;
    const std::vector<Bubble>& getBubbles() const;

    bool isStarted() const;
};

#endif // COORDSCALCULATOR_H
