#include "coordscalculator.h"
#include <math.h>
#include <thread>
#include "locker.h"
#include <QDebug>
#include <QDateTime>

double F(double r) {return 1./r - 1./r/r;}

CoordsCalculator::CoordsCalculator(std::shared_ptr<Locker> locker):
    started_(false),selectedIndex_(-1),locker_(locker)
{

}

CoordsCalculator::~CoordsCalculator()
{

}

// начать расчет
void CoordsCalculator::start()
{
    started_ = true;
    _calculateCoords();
}

// остановить расчет
void CoordsCalculator::stop()
{
    started_ = false;
}

// выделить шарик (не учитывать его при расчетах)
void CoordsCalculator::selectBubble(size_t index)
{
    std::lock_guard<std::mutex> lock(mutex_);
    selectedIndex_ = index;
}

// сбросить выделение
void CoordsCalculator::deselectBubble(bool moved, std::pair<double, double> dst)
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

// удалить все шарики
void CoordsCalculator::removeAllBubbles()
{
    bubbles_.clear();
}

// добавить шарик
void CoordsCalculator::addBubble(const Bubble &bubble)
{
    std::lock_guard<std::mutex> lock(mutex_);
    bubbles_.push_back(bubble);
}

// вернуть количество шариков
size_t CoordsCalculator::getBubblesCount() const
{
    return bubbles_.size();
}

// вернуть вектор шариков
const std::vector<Bubble> &CoordsCalculator::getBubbles() const
{
    return bubbles_;
}

// вернуть состояние
bool CoordsCalculator::isStarted() const
{
    return started_;
}

// расчет координат
void CoordsCalculator::_calculateCoords()
{
    while(started_)
    {
        {
            std::lock_guard<std::mutex> lock(mutex_);

           // qDebug() << "begin\t" << QDateTime::currentMSecsSinceEpoch();

            const size_t count = bubbles_.size();

            // создаем таблицу, в ячейках будем хранить значения dx,dy,r и f
            std::tuple<double,double,double,double> **matrix = new std::tuple<double,double,double,double>*[count];
            for(size_t row=0; row<count; ++row)
                matrix[row] = new std::tuple<double,double,double,double>[count];

            // расчитываем расстояния и модули сил и помещаем в таблицу
            for(size_t column=0; column<count; ++column)
                for(size_t row=column+1; row<count; ++row)
                {
                    std::get<0>(matrix[column][row]) = (bubbles_.at(row).x - bubbles_.at(column).x)/PX2LENGHT;  // dx
                    std::get<1>(matrix[column][row]) = (bubbles_.at(row).y - bubbles_.at(column).y)/PX2LENGHT;  // dy
                    std::get<2>(matrix[column][row]) = sqrt(pow(std::get<0>(matrix[column][row]),2)+pow(std::get<1>(matrix[column][row]),2)); // r

                    // если шарик не выбран, он участвует в расчетах
                    if(column != selectedIndex_ && row != selectedIndex_)
                        std::get<3>(matrix[column][row]) = F(std::get<2>(matrix[column][row])); // f

                    // если шарик выбран, он не взаимодействует с другими шариками, поэтому сила равна 0
                    else
                        std::get<3>(matrix[column][row]) = 0; // f

                    // заполняем вторую половину матрицы
                    std::get<0>(matrix[row][column]) = -std::get<0>(matrix[column][row]);   // dx инвертируем
                    std::get<1>(matrix[row][column]) = -std::get<1>(matrix[column][row]);   // dy инвертируем
                    std::get<2>(matrix[row][column]) = std::get<2>(matrix[column][row]);
                    std::get<3>(matrix[row][column]) = std::get<3>(matrix[column][row]);

                    fprintf(stderr,"%.26f\n",std::get<2>(matrix[column][row]));
                }

            // расчитываем равнодействующие силы
            std::vector<std::pair<double,double>> forces;

            for(size_t iter=0; iter<count; ++iter) // идем по списку точек
            {
                double fx = 0;    // равнодействующая вдоль x
                double fy = 0;    // равнодействующая вдоль y

                for(size_t oter=0; oter<count; ++oter)  // идем по списку остальных точек
                    if(iter != oter)
                    {
                        // fx = f*dx/r
                        fx += std::get<3>(matrix[iter][oter])*std::get<0>(matrix[iter][oter])/std::get<2>(matrix[iter][oter]);

                        // fy = f*dy/r
                        fy += std::get<3>(matrix[iter][oter])*std::get<1>(matrix[iter][oter])/std::get<2>(matrix[iter][oter]);
                    }

                forces.push_back(std::make_pair(fx,fy));
            }

            // удаляем таблицу
            for(size_t row=0; row<count; ++row)
               delete []  matrix[row];

            delete [] matrix;


            // двигаем точки
            for(size_t iter=0; iter<count; ++iter) // идем по списку точек
            {
                Bubble current = bubbles_[iter];

                double t = DELAY/100.;

                // s(t) = s0 + v0*t + at^2/2
                current.x += current.vx*t + forces[iter].first/WEIGHT*t*t/2;
                current.y += current.vy*t + forces[iter].second/WEIGHT*t*t/2;

                // v(t) = v0 + at
                current.vx += forces[iter].first*t;
                current.vy += forces[iter].second*t;

                bubbles_[iter] = current;
            }
          //  qDebug() << "end\t" << QDateTime::currentMSecsSinceEpoch();
            locker_->signal();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }

    locker_->signal();
}
