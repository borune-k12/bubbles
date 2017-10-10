#ifndef COORDSCALCULATOR_H
#define COORDSCALCULATOR_H

#include <memory>
#include <vector>
#include <mutex>
#include "bubble.h"

class Locker;

const int DELAY = 5;                // задержка между тактами
const double DT = 5E-5;             // длительность такта

/**
 * @brief The CoordsCalculator class Класс для расчета координат шариков
 */
class CoordsCalculator
{
private:
    std::vector<Bubble> bubbles_;
    bool started_;
    std::mutex mutex_;
    int selectedIndex_;
    std::shared_ptr<Locker> locker_;
    void _calculateCoords();

public:
    explicit CoordsCalculator(std::shared_ptr<Locker> locker);
    virtual ~CoordsCalculator();

    void start();
    void stop();

    void selectBubble(int index);
    void deselectBubble(bool moved, std::pair<double,double> dst = std::make_pair<double,double>(-1,-1));

    void removeAllBubbles();
    void addBubble(const Bubble& bubble);

    int getBubblesCount() const;
    const std::vector<Bubble>& getBubbles() const;

    bool isStarted() const;
};

#endif // COORDSCALCULATOR_H
