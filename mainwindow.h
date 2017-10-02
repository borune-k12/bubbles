#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "container/threadsafeset.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:

    ThreadSafeSet *points_;

public:
    MainWindow(ThreadSafeSet *points, QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
