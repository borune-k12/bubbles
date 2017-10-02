#ifndef COORDSCALCULATOR_H
#define COORDSCALCULATOR_H

#include "container/threadsafeset.h"

/** Класс для расчета координат шариков */

#define DELAY 10

class CoordsCalculator
{

private:
    ThreadSafeSet *points_;
    bool started_;

    void _calulateCoords();

public:
    explicit CoordsCalculator(ThreadSafeSet *points);
    virtual ~CoordsCalculator();

    void start();
    void stop();

    inline bool isStarted() const;
};

#endif // COORDSCALCULATOR_H
