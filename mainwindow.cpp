#include "mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <unordered_set>

MainWindow::MainWindow(ThreadSafeSet *points, QWidget *parent)
    : QMainWindow(parent),points_(points)

{


}

MainWindow::~MainWindow()
{

}
