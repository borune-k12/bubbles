#include "mainwindow.h"
#include <QDebug>
#include <thread>
#include <QTimer>
#include <QDateTime>
#include <QMenuBar>
#include <QApplication>
#include <QInputDialog>
#include <QMessageBox>
#include <QMenu>

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QPushButton>

#include "graphics/bubblegraphicsitem.h"
#include "graphics/bubblescene.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),thread_(nullptr),bubblesCount_(DEFAULT_BUBBLES_COUNT),clickedIndex_(-1)
{
    mainWidget_ = new QWidget;

    layout_ = new QVBoxLayout;
    graphicsScene_ = new BubbleScene;

    connect(graphicsScene_,&BubbleScene::addBubble,this,&MainWindow::sltAddBubble);
    //graphicsScene_->setSceneRect(0,0,500,500);
    graphicsView_ = new QGraphicsView(graphicsScene_);
    graphicsView_->setRenderHints(QPainter::Antialiasing);
    graphicsView_->adjustSize();

    btn_ = new QPushButton(tr("Начать"));
    connect(btn_,&QPushButton::clicked,this,&MainWindow::sltStartStop);

    layout_->addWidget(graphicsView_);
    layout_->addWidget(btn_);

    mainMenu_ = menuBar()->addMenu(QObject::tr("Меню"));

    mainMenu_->addAction(tr("Настройки"),this,SLOT(sltShowSettings()));
    mainMenu_->addAction(tr("Перемешать"),this,SLOT(sltCreateBubbles()));
    mainMenu_->addAction(tr("Выход"), qApp, SLOT(quit()));

    mainWidget_->setLayout(layout_);

    setCentralWidget(mainWidget_);

    locker_ = std::make_shared<Locker>(false);

    calculator_ = std::unique_ptr<CoordsCalculator>(new CoordsCalculator(locker_));
    sltCreateBubbles();
}

MainWindow::~MainWindow()
{
    if(calculator_->isStarted())
    {
        calculator_->stop();
        thread_->join();
        thread_.reset(nullptr);
    }
    calculator_.reset(nullptr);
}


// начать/остановить симуляцию
void MainWindow::sltStartStop()
{
    // если симуляция запущена - останавливаем ее
    if(calculator_->isStarted())
    {
        calculator_->stop();
        thread_->join();
        thread_.reset(nullptr);
        btn_->setText(tr("Начать"));
    } else {
        thread_ = std::unique_ptr<std::thread>(new std::thread([this]{
            calculator_->start();
        }));
        btn_->setText(tr("Остановить"));
        sltReDraw();
    }
}

// обновление положения шариков
void MainWindow::sltReDraw()
{
    locker_->wait();
    // получаем рассчитанные координаты
    std::vector<Bubble> bubbles = calculator_->getBubbles();
    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);

    // идем по списку элементов сцены
    for(auto iter=0; iter<items.size(); ++iter)
    {
        // если элемент не выбран - перемещаем его в расчитанные координаты
        if(!items[iter]->isSelected())
            items[iter]->setPos(bubbles.at(iter).x,bubbles.at(iter).y);
    }

    // если симуляция запущена - запланируем перезапуск функции обновления
    if(calculator_->isStarted())
        QTimer::singleShot(10,this,SLOT(sltReDraw()));
}

// слот для обработки перемещения шарика
void MainWindow::sltBubbleMoved()
{
    // получаем указатель на элемент, который переместили
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    // получаем координаты элемента
    QPointF center = sndr->getCenter();

    qDebug() << sndr->getIndex() << "moved to"  << center;

    // добавляем новый шарик
    //bubbles_->addElement(sndr->getIndex(),Bubble(center.x(),center.y()));
    calculator_->deselectBubble(true,std::make_pair<double,double>(center.x(),center.y()));
}

// слот для обработки удаления шарика
void MainWindow::sltDeleteBubble()
{
    // получаем указатель на элемент, который надо удалить
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    // удаляем элемент
    graphicsScene_->removeItem(sndr);

    calculator_->deselectBubble(false);

    qDebug() << sndr->getIndex() << "removed" ;

    // обновляем индексы у оставшихся элементов
    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);
    std::for_each(items.begin()+sndr->getIndex(),items.end(),[](QGraphicsItem*item){dynamic_cast<BubbleGraphicsItem*>(item)->updateIndex();});

    qDebug() << sndr->getIndex() << "removed" << items.size() << "left";
    // проверяем, остались ли еще шарики
    if(items.size() == 0) {
        // если шариков больше нет останавливаем симуляцию
        if(calculator_->isStarted())
            sltStartStop();

        // показываем уведомление
        QMessageBox msgBox(QMessageBox::Warning,tr("Сообщение"),tr("Шариков больше не осталось"));
        msgBox.exec();

        // деактивируем кнопку запуска симуляции
        btn_->setEnabled(false);
    }
}

// слот для обработки клика по шарику
void MainWindow::sltBubbleClicked()
{
    // получаем указатель на элемент, на который кликнули
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    qDebug() << sndr->getIndex() << "clicked";

    calculator_->selectBubble(sndr->getIndex());
}

// слот для добавления шарика
void MainWindow::sltAddBubble(const QPointF &dst)
{
    // добавляем шарик
    calculator_->addBubble(Bubble(dst.x(),dst.y()));

    // создаем элемент
    BubbleGraphicsItem *bubble = new BubbleGraphicsItem(calculator_->getBubblesCount()-1,BUBBLE_RADIUS,dst,Qt::blue);
    connect(bubble,&BubbleGraphicsItem::bubbleMoved,this,&MainWindow::sltBubbleMoved);
    connect(bubble,&BubbleGraphicsItem::bubbleDeleted,this,&MainWindow::sltDeleteBubble);
    connect(bubble,&BubbleGraphicsItem::bubbleClicked,this,&MainWindow::sltBubbleClicked);

    // добавляем элемент на сцену
    graphicsScene_->addItem(bubble);

    // для начала симуляции необходимо как минимум 2 шарика
    if(graphicsScene_->items().size()>1)
        btn_->setEnabled(true);
}

// слот для отображения диалога настроек количества шариков
void MainWindow::sltShowSettings()
{
    bool ok;
    int cnt = QInputDialog::getInt(this, tr("Настройки"),tr("Начальное количество шариков"),DEFAULT_BUBBLES_COUNT,2,1000,1, &ok);
    if (ok)
    {

        bubblesCount_ = cnt;
        sltCreateBubbles();
    }
}

// слот для генерации шариков
void MainWindow::sltCreateBubbles()
{
    if(calculator_->isStarted())
        sltStartStop();
    // инициалируем ГПСЧ
    qsrand (QDateTime::currentMSecsSinceEpoch());

    // удаляем все имеющиеся шарики
    calculator_->removeAllBubbles();

    // очищаем сцену
    graphicsScene_->clear();

    /*for(auto iter=0; iter<bubblesCount_; ++iter)
    {
        // добавляем шарик в контейнер
        Bubble bubble(qrand()%graphicsView_->width(),qrand()%graphicsView_->height());
        calculator_->addBubble(bubble);

        // создаем элемент
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(iter,BUBBLE_RADIUS,QPointF(bubble.x,bubble.y),Qt::blue);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleMoved,this,&MainWindow::sltBubbleMoved);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleDeleted,this,&MainWindow::sltDeleteBubble);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleClicked,this,&MainWindow::sltBubbleClicked);

        // добавляем элемент на сцену
        graphicsScene_->addItem(bubbleItem);
    }*/


    Bubble bubble(100+0.5*200,100);
    calculator_->addBubble(bubble);

    // создаем элемент
    BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(0,BUBBLE_RADIUS,QPointF(bubble.x,bubble.y),Qt::blue);
    // добавляем элемент на сцену
    graphicsScene_->addItem(bubbleItem);

    bubble = Bubble(100,100+200*sqrt(3)/2);
    calculator_->addBubble(bubble);

    // создаем элемент
    bubbleItem = new BubbleGraphicsItem(1,BUBBLE_RADIUS,QPointF(bubble.x,bubble.y),Qt::blue);
        // добавляем элемент на сцену
    graphicsScene_->addItem(bubbleItem);

    bubble = Bubble(100+1*200,100+200*sqrt(3)/2);
    calculator_->addBubble(bubble);
    bubbleItem = new BubbleGraphicsItem(1,BUBBLE_RADIUS,QPointF(bubble.x,bubble.y),Qt::blue);
        // добавляем элемент на сцену
    graphicsScene_->addItem(bubbleItem);



     btn_->setEnabled(true);
}
