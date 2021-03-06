#include <gmock/gmock.h>
#include "movementcalculator.h"
#include "testshapes.h"

const int TEST_STEPS_COUNT = 1E6;   // количество итераций тестирования
const double MAX_DELTA = 1E-10;     // погрешность
const double DT = 2E-5;             // длительность такта. Для повышения точности расчетов необходимо уменьшать данный параметр

/**
 * @brief test функция тестирования
 * @param bubbles вектор шариков
 */
void test(std::vector<Bubble> &bubbles) {
    for(auto iter=0; iter<TEST_STEPS_COUNT; ++iter)
    {
        // производим шаг симуляции
        MovementCalculator::doStep(bubbles,DT);

        // рассчитываем расстояния между последовательными парами шариков (1-2, 2-3, 3-4, ...)
        std::vector<double> rs;

        for(auto i=0; i<bubbles.size(); ++i)
        {
            double r;
            if(i == bubbles.size()-1)
            {
                double dx = bubbles.at(i).x - bubbles.at(0).x;
                double dy = bubbles.at(i).y - bubbles.at(0).y;
                r = sqrt(pow(dx,2)+pow(dy,2));
            } else
            {
                double dx = bubbles.at(i).x - bubbles.at(i+1).x;
                double dy = bubbles.at(i).y - bubbles.at(i+1).y;
                r = sqrt(pow(dx,2)+pow(dy,2));
            }

            rs.push_back(r);
        }

        // поскольку фигура правильная, то расстояние между шариками должно быть одинаковым в любой момент времени
        for(auto i=0; i<rs.size()-1; ++i)
            EXPECT_NEAR(rs.at(i),rs.at(i+1),MAX_DELTA);
    }
}

/**
 * @brief TEST тестирование на треугольнике
 */
TEST(CalculatorTest,Triangle) {

    std::vector<Bubble> bubbles;

    std::for_each(TestShapes::triangle.begin(),TestShapes::triangle.end(),[&bubbles](const Point &point){
        bubbles.push_back(Bubble(point.first,point.second));
    });

    test(bubbles);
}

/**
 * @brief TEST тестирование на квадрате
 */
TEST(CalculatorTest,Square) {

    std::vector<Bubble> bubbles;

    std::for_each(TestShapes::square.begin(),TestShapes::square.end(),[&bubbles](const Point &point){
        bubbles.push_back(Bubble(point.first,point.second));
    });

    test(bubbles);
}

/**
 * @brief TEST тестирование на шестиугольнике
 */
TEST(CalculatorTest,Hexagon) {

    std::vector<Bubble> bubbles;

    std::for_each(TestShapes::hexagon.begin(),TestShapes::hexagon.end(),[&bubbles](const Point &point){
        bubbles.push_back(Bubble(point.first,point.second));
    });

    test(bubbles);
}

/**
 * @brief TEST тестирование на паре шариков
 */
TEST(CalculatorTest,Pair) {

    std::vector<Bubble> bubbles;

    std::for_each(TestShapes::pair.begin(),TestShapes::pair.end(),[&bubbles](const Point &point){
        bubbles.push_back(Bubble(point.first,point.second));
    });

    for(auto iter=0; iter<TEST_STEPS_COUNT; ++iter)
    {
        // сохраняем координаты шариков
        double x1 = bubbles.at(0).x;
        double y1 = bubbles.at(0).y;
        double x2 = bubbles.at(1).x;
        double y2 = bubbles.at(1).y;

        // производим шаг симуляции
        MovementCalculator::doStep(bubbles,DT);

        /*  так как расстояние между шариками 1 и начальных скоростей нет, то сила равна 0,
         ** следовательно, шарики должны остаться на своих местах */
        EXPECT_EQ(x1,bubbles.at(0).x);
        EXPECT_EQ(y1,bubbles.at(0).y);
        EXPECT_EQ(x2,bubbles.at(1).x);
        EXPECT_EQ(y2,bubbles.at(1).y);
    }
}


int main(int argc, char** argv)
{
    testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}


