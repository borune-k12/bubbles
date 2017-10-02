#include "coordscalculator.h"
#include <math.h>
#include <thread>

CoordsCalculator::CoordsCalculator(ThreadSafeSet *points):
    points_(points),started_(false)
{

}

CoordsCalculator::~CoordsCalculator()
{

}

// начать расчет
void CoordsCalculator::start()
{
    started_ = true;
    _calulateCoords();
}

// остановить расчет
void CoordsCalculator::stop()
{
    started_ = false;
}

// вернуть состояние
bool CoordsCalculator::isStarted() const
{
    return started_;
}

// непосредственный расчет координат
void CoordsCalculator::_calulateCoords()
{
    while(started_)
    {

        double alpha = atan2(-2,-1)*180/M_PI;
        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}
