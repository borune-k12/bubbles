#include "coordscalculator.h"
#include <math.h>
#include <thread>
#include "locker.h"
#include "movementcalculator.h"

/**
 * @brief CoordsCalculator::CoordsCalculator
 * @param locker: указатель на семафор
 */
CoordsCalculator::CoordsCalculator(std::shared_ptr<Locker> locker):
    started_(false),selectedIndex_(-1),locker_(locker)
{

}

CoordsCalculator::~CoordsCalculator()
{

}

/**
 * @brief CoordsCalculator::start начать расчет
 */
void CoordsCalculator::start()
{
    started_ = true;
    _calculateCoords();
}

/**
 * @brief CoordsCalculator::stop остановить расчет
 */
void CoordsCalculator::stop()
{
    started_ = false;
}

/**
 * @brief CoordsCalculator::selectBubble выделить шарик (не учитывать его при расчетах)
 * @param index: индекс выбранного шарика
 */
void CoordsCalculator::selectBubble(int index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    selectedIndex_ = index;
}

/**
 * @brief CoordsCalculator::deselectBubble сбросить выделение
 * @param moved флаг перемещения шарика. true - шарик перетащили, false - шарик лопнули
 * @param dst новые координаты шарика (если его перетащили)
 */
void CoordsCalculator::deselectBubble(bool moved, Point dst)
{
    std::lock_guard<std::mutex> lock(mutex_);
    if(moved)
        // шарик переместили
        bubbles_[selectedIndex_] = Bubble(dst.first,dst.second);
    else
        // шарик удалили
        bubbles_.erase(bubbles_.begin()+selectedIndex_);

    selectedIndex_ = -1;
}

/**
 * @brief CoordsCalculator::removeAllBubbles удалить все шарики
 */
void CoordsCalculator::removeAllBubbles()
{
    std::lock_guard<std::mutex> lock(mutex_);
    bubbles_.clear();
}

/**
 * @brief CoordsCalculator::addBubble добавить шарик
 * @param bubble добавляемый объект
 */
void CoordsCalculator::addBubble(const Bubble &bubble)
{
    std::lock_guard<std::mutex> lock(mutex_);
    bubbles_.push_back(bubble);
}

/**
 * @brief CoordsCalculator::getBubblesCount вернуть количество шариков
 * @return количество шариков в векторе
 */
int CoordsCalculator::getBubblesCount() const
{
    return bubbles_.size();
}

/**
 * @brief CoordsCalculator::getBubbles вернуть вектор шариков
 * @return вектор шариков
 */
const std::vector<Bubble> &CoordsCalculator::getBubbles() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return bubbles_;
}

/**
 * @brief CoordsCalculator::isStarted вернуть состояние симуляции
 * @return true - выполняется, false - остановлена
 */
bool CoordsCalculator::isStarted() const
{
    return started_;
}

/**
 * @brief CoordsCalculator::_calculateCoords расчет координат
 */
void CoordsCalculator::_calculateCoords()
{
    while(started_)
    {
        // на время вычисления блокируем доступ к вектору
        {
            std::lock_guard<std::mutex> lock(mutex_);
            MovementCalculator::doStep(bubbles_,DT,selectedIndex_);

            // освобождаем семафор
            locker_->signal();
        }

        std::this_thread::sleep_for(std::chrono::microseconds(DELAY));
    }

    // "отпускаем" графику после завершения симуляции
    locker_->signal();
}
