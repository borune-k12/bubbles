#ifndef MOVEMENTCALCULATOR_H
#define MOVEMENTCALCULATOR_H

#include <vector>
#include <bubble.h>

namespace MovementCalculator
{
    /**
     * @brief F: расчет модуля силы
     * @param r: расстояние между двумя шариками
     * @return величину модуля силы
     */
    double F(double r) {return 1./r - 1./r/r;}

    /**
     * @brief doStep: выполнение шага симуляции
     * @param bubbles_: вектор шариков
     * @param dt: временной интервал, в течение которого осуществлялось движение
     * @param selectedIndex_: индекс элемента в массиве, который не участвует в расчетах (нужен, когда шарик перетаскивают)
     */
    static void doStep(std::vector<Bubble> &bubbles_, const double dt, const int selectedIndex_ = -1) {
        const int count = bubbles_.size();

        // создаем таблицу, в ячейках будем хранить значения dx,dy,r и f
        std::tuple<double,double,double,double> **matrix = new std::tuple<double,double,double,double>*[count];
        for(int row=0; row<count; ++row)
            matrix[row] = new std::tuple<double,double,double,double>[count];

        // расчитываем расстояния и модули сил и помещаем в таблицу
        for(int column=0; column<count; ++column)
            for(int row=column+1; row<count; ++row)
            {
                std::get<0>(matrix[column][row]) = bubbles_.at(row).x - bubbles_.at(column).x;  // dx
                std::get<1>(matrix[column][row]) = bubbles_.at(row).y - bubbles_.at(column).y;  // dy
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
            }

        // расчитываем равнодействующие силы
        std::vector<std::pair<double,double>> forces;

        for(int iter=0; iter<count; ++iter) // идем по списку точек
        {
            double fx = 0;    // равнодействующая вдоль x
            double fy = 0;    // равнодействующая вдоль y

            for(int oter=0; oter<count; ++oter)  // идем по списку остальных точек
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
        for(int row=0; row<count; ++row)
           delete []  matrix[row];

        delete [] matrix;

        // двигаем точки
        for(int iter=0; iter<count; ++iter) // идем по списку точек
        {
            Bubble current = bubbles_[iter];

            // чем меньше будет значение выражения dt/BUBBLE_WEIGHT, тем меньше будет погрешность

            // s(t) = s0 + v0*t + at^2/2
            current.x += current.vx*dt + forces[iter].first/BUBBLE_WEIGHT*dt*dt/2;
            current.y += current.vy*dt + forces[iter].second/BUBBLE_WEIGHT*dt*dt/2;

            // v(t) = v0 + at
            current.vx += forces[iter].first/BUBBLE_WEIGHT*dt;
            current.vy += forces[iter].second/BUBBLE_WEIGHT*dt;

            bubbles_[iter] = current;
        }
    }
}

#endif // MOVEMENTCALCULATOR_H
