#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>
#include "container/threadsafevector.h"
#include "coordscalculator.h"
#include <thread>

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ThreadSafeVector<Bubble> *points_;
    QGraphicsScene *graphicsScene_;
    QGraphicsView *graphicsView_;
    QVBoxLayout *layout;
    QWidget *mainWidget;
    CoordsCalculator *calculator_;

    QPushButton *btn;
    std::thread *thread_;

public:
    MainWindow(ThreadSafeVector<Bubble> *points, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void start();
    void _redraw();

};

#endif // MAINWINDOW_H
