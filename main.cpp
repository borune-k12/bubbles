#include "mainwindow.h"
#include <QApplication>


ThreadSafeVector<Bubble> points;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    for(auto i=0; i<2; ++i) {
        points.addElement(Bubble(i*100,i*100));
    }

    MainWindow w(&points);
    w.show();


    return a.exec();
}
