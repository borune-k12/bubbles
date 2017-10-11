#ifndef COORDSCALCULATOR_H
#define COORDSCALCULATOR_H

#include <memory>
#include <vector>
#include <mutex>
#include "bubble.h"
#include "point.h"

class Locker;

const int DELAY = 10;               // задержка между тактами
const double DT = 2E-5;             // длительность такта. Для повышения точности расчетов необходимо уменьшать данный параметр

/**
 * @brief The CoordsCalculator class класс для расчета координат шариков
 */
class CoordsCalculator
{
private:
    std::vector<Bubble> bubbles_;       // вектор шариков
    bool started_;                      // флаг запуска
    mutable std::mutex mutex_;          // мьютекс для блокировки
    int selectedIndex_;                 // индекс выбранного шарика
    std::shared_ptr<Locker> locker_;    // семафор

    void _calculateCoords();

public:
    explicit CoordsCalculator(std::shared_ptr<Locker> locker);
    virtual ~CoordsCalculator();

    void start();
    void stop();

    void selectBubble(int index);
    void deselectBubble(bool moved, Point dst = Point(-1,-1));
    void addBubble(const Bubble& bubble);

    void removeAllBubbles();

    const std::vector<Bubble>& getBubbles() const;
    int getBubblesCount() const;

    bool isStarted() const;
};

#endif // COORDSCALCULATOR_H
