#include "coordscalculator.h"
#include <math.h>
#include <thread>
#include <QDebug>
#include <sstream>
#include <string>

#define F(r) ({1/r - 1/r/r;})

CoordsCalculator::CoordsCalculator(ThreadSafeVector<Bubble> *points):
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
    _calculateCoords();
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

// расчет координат
void CoordsCalculator::_calculateCoords()
{
    while(started_)
    {
        // расчитываем расстояния и модули сил и помещаем в таблицу
        size_t count = points_->getElementsCount();
        auto bubbles_vector = points_->getElements();
        std::tuple<float,float,float,float> matrix[count][count]; // dx,dy,r,f

        for(size_t column=0; column<count; ++column)
            for(size_t row=column+1; row<count; ++row)
            {
                std::get<0>(matrix[column][row]) = (bubbles_vector.at(row).x - bubbles_vector.at(column).x)/PX2LENGHT;  // dx
                std::get<1>(matrix[column][row]) = (bubbles_vector.at(row).y - bubbles_vector.at(column).y)/PX2LENGHT;  // dy
                std::get<2>(matrix[column][row]) = sqrt(pow(std::get<0>(matrix[column][row]),2)+pow(std::get<1>(matrix[column][row]),2)); // r
                std::get<3>(matrix[column][row]) = F(std::get<2>(matrix[column][row])); // f

                // заполняем вторую половину матрицы
                std::get<0>(matrix[row][column]) = -std::get<0>(matrix[column][row]);   // dx инвертируем
                std::get<1>(matrix[row][column]) = -std::get<1>(matrix[column][row]);   // dy инвертируем
                std::get<2>(matrix[row][column]) = std::get<2>(matrix[column][row]);
                std::get<3>(matrix[row][column]) = std::get<3>(matrix[column][row]);

               // qDebug() << "dx = " << std::get<0>(matrix[column][row]);
                //qDebug() << "dy = " << std::get<1>(matrix[column][row]);
                /*static float max_r = std::get<2>(matrix[row][column]);
                if(max_r < std::get<2>(matrix[row][column])) {
                    qDebug() << "r = " << std::get<2>(matrix[row][column]);
                    max_r = std::get<2>(matrix[row][column]);
                }*/
                //qDebug() << "f = " << std::get<3>(matrix[row][column]);
            }

        // расчитываем равнодействующие силы
        std::vector<std::pair<float,float>> forces;

        for(size_t iter=0; iter<count; ++iter) // идем по списку точек
        {
            float fx = 0;    // равнодействующая вдоль x
            float fy = 0;    // равнодействующая вдоль y

            for(size_t oter=0; oter<count; ++oter)  // идем по списку остальных точек
                if(iter != oter)
                {
                    // fx = f*dx/r
                    fx += std::get<3>(matrix[iter][oter])*std::get<0>(matrix[iter][oter])/std::get<2>(matrix[iter][oter]);

                    // fy = f*dy/r
                    fy += std::get<3>(matrix[iter][oter])*std::get<1>(matrix[iter][oter])/std::get<2>(matrix[iter][oter]);
                }

            //qDebug() << iter << ": fx = " << fx;
            //qDebug() << iter << ": fy = " << fy;
            forces.push_back(std::make_pair(fx,fy));
        }

        // двигаем точки
        for(size_t iter=0; iter<count; ++iter) // идем по списку точек
        {
            Bubble current = bubbles_vector[iter];

            float t = DELAY/100.;
            current.x += current.vx*t + forces[iter].first/WEIGHT*t*t/2;
            current.y += current.vy*t + forces[iter].second/WEIGHT*t*t/2;

            current.vx += forces[iter].first*t;
            current.vy += forces[iter].second*t;


            //qDebug() << iter << ": vx = " << current.vx;
            //qDebug() << iter << ": vy = " << current.vy;

            points_->setElement(iter,current);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(DELAY));
    }
}
