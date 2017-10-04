#include "mainwindow.h"
#include <QDateTime>
#include <QDebug>
#include <unordered_set>
#include <thread>
#include <QTimer>
#include <QMenuBar>
#include <QApplication>
#include <QInputDialog>

#include "graphics/bubblegraphicsitem.h"
#include "graphics/bubblescene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),thread_(nullptr),bubblesCount_(DEFAULT_BUBBLES_COUNT)
{
    mainWidget_ = new QWidget;

    layout_ = new QVBoxLayout;
    graphicsScene_ = new BubbleScene;

    connect(graphicsScene_,&BubbleScene::addBubble,this,&MainWindow::addBubble);
    graphicsScene_->setSceneRect(0,0,800,600);
    graphicsView_ = new QGraphicsView(graphicsScene_);
    graphicsView_->setRenderHints(QPainter::Antialiasing);

    btn_ = new QPushButton(tr("Начать"));
    connect(btn_,&QPushButton::clicked,this,&MainWindow::start);

    layout_->addWidget(graphicsView_);
    layout_->addWidget(btn_);


    mainMenu_ = menuBar()->addMenu(QObject::tr("Меню"));

    mainMenu_->addAction(tr("Настройки"),this,SLOT(showSettings()));
    mainMenu_->addAction(tr("Перемешать"),this,SLOT(_createBubbles()));
    mainMenu_->addAction(tr("Выход"), qApp, SLOT(quit()));

    mainWidget_->setLayout(layout_);

    setCentralWidget(mainWidget_);

    bubbles_ = new ThreadSafeVector<Bubble>();
    calculator_ = new CoordsCalculator(bubbles_);
    _createBubbles();
}

MainWindow::~MainWindow()
{
    if(calculator_->isStarted())
    {
        calculator_->stop();
        thread_->join();
        delete thread_;
    }
    delete calculator_;
}


void MainWindow::start()
{
    if(thread_ != nullptr)
    {
        calculator_->stop();
        thread_->join();
        delete thread_;
        thread_ = nullptr;
        btn_->setText(tr("Начать"));
    } else {
        thread_ = new std::thread([this]{
            calculator_->start();
        });
        btn_->setText(tr("Остановить"));
        redraw();
    }
}

void MainWindow::redraw()
{
    std::vector<Bubble> bubbles = bubbles_->getElements();

    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);
    for(auto iter=0; iter<items.size(); ++iter)
    {
        QGraphicsItem* currentItem = items[iter];
        currentItem->setPos(bubbles.at(iter).x,bubbles.at(iter).y);
    }

    if(thread_ != nullptr)
        QTimer::singleShot(DELAY,this,SLOT(redraw()));
}

void MainWindow::bubbleMoved(size_t index)
{
    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);
    QPointF destPos = items[index]->pos();
    qDebug() << index << " moved to " << destPos.x() << ";" << destPos.y();

    items[index]->setPos(destPos);
    bubbles_->addElementToPosition(index,Bubble(destPos.x(),destPos.y()));
}

void MainWindow::bubbleDeleted(size_t index)
{
    qDebug() << index << " deleted";
    bubbles_->removeElement(index);
}

void MainWindow::addBubble(const QPointF &dst)
{
    bubbles_->addElement(Bubble(dst.x(),dst.y()));

    BubbleGraphicsItem *bubble = new BubbleGraphicsItem(bubbles_->getElementsCount()-1,BUBBLE_RADIUS,dst,Qt::blue);
    connect(bubble,&BubbleGraphicsItem::bubbleMoved,this,&MainWindow::bubbleMoved);
    connect(bubble,&BubbleGraphicsItem::bubbleDeleted,this,&MainWindow::bubbleDeleted);

    graphicsScene_->addItem(bubble);
}

void MainWindow::showSettings()
{
    bool ok;
    int cnt = QInputDialog::getInt(this, tr("Настройки"),
                                         tr("Начальное количество шариков"),DEFAULT_BUBBLES_COUNT,2,500,1, &ok);
    if (ok) {
        bubblesCount_ = cnt;
        _createBubbles();
    }
}

void MainWindow::_createBubbles()
{
    // инициалируем ГПСЧ
    qsrand (QDateTime::currentMSecsSinceEpoch());

    // удаляем все шарики
    bubbles_->clear();
    graphicsScene_->clear();

    for(auto iter=0; iter<bubblesCount_; ++iter)
    {
        // добавляем элементы в контейнер
        Bubble bubble(qrand()%800,qrand()%600);

        bubbles_->addElement(bubble);

        // добавляем элементы на сцену
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(iter,BUBBLE_RADIUS,QPointF(bubble.x,bubble.y),Qt::blue);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleMoved,this,&MainWindow::bubbleMoved);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleDeleted,this,&MainWindow::bubbleDeleted);

        graphicsScene_->addItem(bubbleItem);

    }
}
