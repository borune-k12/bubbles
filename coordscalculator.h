#ifndef COORDSCALCULATOR_H
#define COORDSCALCULATOR_H

#include "container/threadsafevector.h"

/** Класс для расчета координат шариков */

#define DELAY 50
#define WEIGHT 1
#define PX2LENGHT 100

class CoordsCalculator
{

private:
    ThreadSafeVector<Bubble> *points_;
    bool started_;

    void _calculateCoords();

public:
    explicit CoordsCalculator(ThreadSafeVector<Bubble> *points);
    virtual ~CoordsCalculator();

    void start();
    void stop();

    inline bool isStarted() const;
};

#endif // COORDSCALCULATOR_H
