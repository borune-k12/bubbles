#include "mainwindow.h"
#include <QApplication>
#include "coordscalculator.h"
#include <thread>


ThreadSafeSet points;
CoordsCalculator calculator(&points);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    MainWindow w(&points);

    std::thread thread([]{
        calculator.start();
    });

    thread.join();
    w.show();

    return a.exec();
}
