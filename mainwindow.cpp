#include "mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <unordered_set>
#include <thread>
#include <QGraphicsItem>
#include <QTimer>

MainWindow::MainWindow(ThreadSafeVector<Bubble> *points, QWidget *parent)
    : QMainWindow(parent),points_(points),thread_(nullptr)
{
    mainWidget = new QWidget;

    layout = new QVBoxLayout;
    graphicsScene_ = new QGraphicsScene;
    graphicsScene_->setSceneRect(0,0,800,600);
    graphicsView_ = new QGraphicsView(graphicsScene_);

    btn = new QPushButton(tr("Start"));
    connect(btn,&QPushButton::clicked,this,&MainWindow::start);

    layout->addWidget(graphicsView_);
    layout->addWidget(btn);

    mainWidget->setLayout(layout);

    std::vector<Bubble> bubbles = points_->getElements();

    for(size_t iter=0; iter<bubbles.size(); ++iter)
        graphicsScene_->addEllipse(bubbles.at(iter).x,bubbles.at(iter).y,5,5);

    setCentralWidget(mainWidget);

    calculator_ = new CoordsCalculator(points_);
}

MainWindow::~MainWindow()
{
    delete calculator_;
}


void MainWindow::start()
{
    if(thread_ != nullptr)
    {
        calculator_->stop();
        thread_->join();
        thread_ = nullptr;
    } else {
        thread_ = new std::thread([this]{
            calculator_->start();
        });
       // _redraw();
    }
}

void MainWindow::_redraw()
{
    std::vector<Bubble> bubbles = points_->getElements();

    QList<QGraphicsItem *> items = graphicsScene_->items();
    for(auto iter=0; iter<items.size(); ++iter)
    {
        QGraphicsItem* currentItem = items[iter];
        currentItem->setPos(bubbles.at(iter).x,bubbles.at(iter).y);
    }

    if(thread_ != nullptr)
        QTimer::singleShot(DELAY,this,SLOT(_redraw()));
}
