#include "mainwindow.h"
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

#include "bubblegraphicsitem.h"
#include "bubblescene.h"

#include "locker.h"
#include "testshapes.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),thread_(nullptr),bubblesCount_(DEFAULT_BUBBLES_COUNT),clickedIndex_(-1)
{
    mainWidget_ = new QWidget;

    layout_ = new QVBoxLayout;
    graphicsScene_ = new BubbleScene;

    connect(graphicsScene_,&BubbleScene::addBubble,this,&MainWindow::sltAddBubble);
    graphicsScene_->setSceneRect(-400,-400,800,800);
    graphicsView_ = new QGraphicsView(graphicsScene_);
    graphicsView_->setRenderHints(QPainter::Antialiasing);


    btn_ = new QPushButton(tr("Начать"));
    connect(btn_,&QPushButton::clicked,this,&MainWindow::sltStartStop);

    layout_->addWidget(graphicsView_);
    layout_->addWidget(btn_);

    mainMenu_ = menuBar()->addMenu(QObject::tr("Меню"));

    mainMenu_->addAction(tr("Настройки"),this,SLOT(sltShowSettings()));

    QMenu *shakeMenu = new QMenu(tr("Перемешать"));
    mainMenu_->addMenu(shakeMenu);

    shakeMenu->addAction(tr("Случайно"),this,SLOT(sltCreateBubblesRandom()));
    shakeMenu->addAction(tr("Треугольник"),this,SLOT(sltCreateBubblesTriangle()));
    shakeMenu->addAction(tr("Квадрат"),this,SLOT(sltCreateBubblesSquare()));
    shakeMenu->addAction(tr("Шестиугольник"),this,SLOT(sltCreateBubblesHexagon()));

    mainMenu_->addAction(tr("Выход"), qApp, SLOT(quit()));

    mainWidget_->setLayout(layout_);

    setCentralWidget(mainWidget_);

    locker_ = std::make_shared<Locker>(false);

    calculator_ = std::unique_ptr<CoordsCalculator>(new CoordsCalculator(locker_));
    sltCreateBubblesRandom();
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

/**
 * @brief MainWindow::sltStartStop начать/остановить симуляцию
 */
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

/**
 * @brief MainWindow::sltReDraw обновление положения шариков
 */
void MainWindow::sltReDraw()
{
    // ждем освобождения семафора
    locker_->wait();

    // получаем рассчитанные координаты
    std::vector<Bubble> bubbles = calculator_->getBubbles();
    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);

    // идем по списку элементов сцены
    for(auto iter=0; iter<items.size(); ++iter)
    {
        // если элемент не выбран - перемещаем его в расчитанные координаты
        if(!items[iter]->isSelected())
            items[iter]->setPos(convertToScene(bubbles.at(iter).x,bubbles.at(iter).y));
    }

    // если симуляция запущена - запланируем перезапуск функции обновления
    if(calculator_->isStarted())
        QTimer::singleShot(10,this,SLOT(sltReDraw()));
}

/**
 * @brief MainWindow::sltBubbleMoved слот для обработки перемещения шарика
 */
void MainWindow::sltBubbleMoved()
{
    // получаем указатель на элемент, который переместили
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    // получаем координаты элемента
    QPointF center = sndr->getCenter();

    // добавляем новый шарик
    calculator_->deselectBubble(true,convertToLocal(center));
}

/**
 * @brief MainWindow::sltDeleteBubble слот для обработки удаления шарика
 */
void MainWindow::sltDeleteBubble()
{
    // получаем указатель на элемент, который надо удалить
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    // удаляем элемент
    graphicsScene_->removeItem(sndr);

    calculator_->deselectBubble(false);

    // обновляем индексы у оставшихся элементов
    QList<QGraphicsItem *> items = graphicsScene_->items(Qt::AscendingOrder);
    std::for_each(items.begin()+sndr->getIndex(),items.end(),[](QGraphicsItem* item){dynamic_cast<BubbleGraphicsItem*>(item)->updateIndex();});

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

/**
 * @brief MainWindow::sltBubbleClicked слот для обработки клика по шарику
 */
void MainWindow::sltBubbleClicked()
{
    // получаем указатель на элемент, на который кликнули
    BubbleGraphicsItem* sndr = dynamic_cast<BubbleGraphicsItem*>(sender());

    calculator_->selectBubble(sndr->getIndex());
}

/**
 * @brief MainWindow::sltAddBubble слот для добавления шарика
 * @param dst координаты точку, в которую надо добавить шарик
 */
void MainWindow::sltAddBubble(const QPointF &dst)
{
    // добавляем шарик
    std::pair<double,double> coords = convertToLocal(dst);

    calculator_->addBubble(Bubble(coords.first,coords.second));

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

/**
 * @brief MainWindow::sltShowSettings слот для отображения диалога настроек количества шариков
 */
void MainWindow::sltShowSettings()
{
    bool ok;
    int cnt = QInputDialog::getInt(this, tr("Настройки"),tr("Начальное количество шариков"),DEFAULT_BUBBLES_COUNT,2,1000,1, &ok);
    if (ok) {
        bubblesCount_ = cnt;
        sltCreateBubblesRandom();
    }
}

/**
 * @brief MainWindow::sltCreateBubbles слот для генерации шариков
 */
void MainWindow::sltCreateBubblesRandom()
{
    if(calculator_->isStarted())
        sltStartStop();

    // инициализируем ГПСЧ
    qsrand (QDateTime::currentMSecsSinceEpoch());

    // удаляем все имеющиеся шарики
    calculator_->removeAllBubbles();

    // очищаем сцену
    graphicsScene_->clear();

    for(auto iter=0; iter<bubblesCount_; ++iter)
    {
        // добавляем шарик в контейнер
        Bubble bubble((qrand()-RAND_MAX/2)/(RAND_MAX+0.),(qrand()-RAND_MAX/2)/(RAND_MAX+0.));
        calculator_->addBubble(bubble);

        // создаем элемент
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(iter,BUBBLE_RADIUS,convertToScene(bubble.x,bubble.y),Qt::blue);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleMoved,this,&MainWindow::sltBubbleMoved);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleDeleted,this,&MainWindow::sltDeleteBubble);
        connect(bubbleItem,&BubbleGraphicsItem::bubbleClicked,this,&MainWindow::sltBubbleClicked);

        // добавляем элемент на сцену
        graphicsScene_->addItem(bubbleItem);
    }

    btn_->setEnabled(true);
}

/**
 * @brief MainWindow::sltCreateBubblesHexagon сделать правильный шестиугольник из шариков
 */
void MainWindow::sltCreateBubblesHexagon() {

    // останавливаем симуляцию
    if(calculator_->isStarted())
        sltStartStop();

    // очищаем сцену
    graphicsScene_->clear();

    // удаляем шарики из калькулятора
    calculator_->removeAllBubbles();

    std::for_each(TestShapes::hexagon.begin(),TestShapes::hexagon.end(),[this](const std::pair<double,double> point){
        // создаем шарик
        Bubble bubble(point.first,point.second);

        // добавляем шарик в калькулятор
        calculator_->addBubble(bubble);

        // создаем элемент
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(0,BUBBLE_RADIUS,convertToScene(bubble.x,bubble.y),Qt::blue);

        // добавляем элемент на сцену
        graphicsScene_->addItem(bubbleItem);
    });

    btn_->setEnabled(true);
}
/**
 * @brief MainWindow::sltCreateBubblesSquare сделать квадрат из шариков
 */
void MainWindow::sltCreateBubblesSquare() {

    // останавливаем симуляцию
    if(calculator_->isStarted())
        sltStartStop();

    // очищаем сцену
    graphicsScene_->clear();

    // удаляем шарики из калькулятора
    calculator_->removeAllBubbles();

    std::for_each(TestShapes::square.begin(),TestShapes::square.end(),[this](const std::pair<double,double> point){
        // создаем шарик
        Bubble bubble(point.first,point.second);

        // добавляем шарик в калькулятор
        calculator_->addBubble(bubble);

        // создаем элемент
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(0,BUBBLE_RADIUS,convertToScene(bubble.x,bubble.y),Qt::blue);

        // добавляем элемент на сцену
        graphicsScene_->addItem(bubbleItem);
    });

    btn_->setEnabled(true);
}

/**
 * @brief MainWindow::sltCreateBubblesTriangle сделать правильный треугольник из шариков
 */
void MainWindow::sltCreateBubblesTriangle() {

    // останавливаем симуляцию
    if(calculator_->isStarted())
        sltStartStop();

    // очищаем сцену
    graphicsScene_->clear();

    // удаляем шарики из калькулятора
    calculator_->removeAllBubbles();

    std::for_each(TestShapes::triangle.begin(),TestShapes::triangle.end(),[this](const std::pair<double,double> point){
        // создаем шарик
        Bubble bubble(point.first,point.second);

        // добавляем шарик в калькулятор
        calculator_->addBubble(bubble);

        // создаем элемент
        BubbleGraphicsItem *bubbleItem = new BubbleGraphicsItem(0,BUBBLE_RADIUS,convertToScene(bubble.x,bubble.y),Qt::blue);

        // добавляем элемент на сцену
        graphicsScene_->addItem(bubbleItem);
    });

    btn_->setEnabled(true);
}

/**
 * @brief MainWindow::convertToScene перевести из координат калькулятора в координаты сцены
 * @param x x координата
 * @param y y координата
 * @return координата точки в системе координат сцены
 */
QPointF MainWindow::convertToScene(double x, double y) {
    return QPointF(x*((graphicsScene_->sceneRect().right()-graphicsScene_->sceneRect().left())/2),
                   -y*(graphicsScene_->sceneRect().bottom()-graphicsScene_->sceneRect().top())/2);
}

/**
 * @brief MainWindow::convertToLocal перевести из координат сцены в координаты калькулятора
 * @param point координаты на сцене
 * @return координата точки в системе координат калькулятора
 */
std::pair<double,double> MainWindow::convertToLocal(const QPointF &point) {
    return std::make_pair<double,double>(2*point.x()/(graphicsScene_->sceneRect().right()-graphicsScene_->sceneRect().left()),
                                         -2*point.y()/(graphicsScene_->sceneRect().bottom()-graphicsScene_->sceneRect().top()));

}
