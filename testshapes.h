#ifndef TESTSHAPES_H
#define TESTSHAPES_H

/**
 * координаты правильных фигур для тестирования
 */
namespace TestShapes {
    // шестиугольник
    const std::vector<std::pair<double,double>> hexagon{
                std::make_pair<double,double>(-0.75/2,-sqrt(3)/4),
                std::make_pair<double,double>(0.25/2,-sqrt(3)/4),
                std::make_pair<double,double>(0.75/2, 0),
                std::make_pair<double,double>(0.25/2,sqrt(3)/4),
                std::make_pair<double,double>(-0.75/2,sqrt(3)/4),
                std::make_pair<double,double>(-1.25/2,0)};

    // квадрат
    const std::vector<std::pair<double,double>> square{
                std::make_pair<double,double>(-0.5,-0.5),
                std::make_pair<double,double>(-0.5,0.5),
                std::make_pair<double,double>(0.5,0.5),
                std::make_pair<double,double>(0.5, -0.5)};

    // треугольник
    const std::vector<std::pair<double,double>> triangle{
                std::make_pair<double,double>(-0.5,0),
                std::make_pair<double,double>(0.25,sqrt(3)/4),
                std::make_pair<double,double>(0.25,-sqrt(3)/4)};
}
#endif // TESTSHAPES_H
