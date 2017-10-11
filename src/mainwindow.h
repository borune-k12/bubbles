#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <thread>
#include <memory>
#include "testshapes.h"

class QGraphicsScene;
class QGraphicsView;
class QVBoxLayout;
class QPushButton;
class QMenu;
class BubbleScene;
class BubbleGraphicsItem;
class Locker;
class CoordsCalculator;

const int DEFAULT_BUBBLES_COUNT = 5;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    BubbleScene *graphicsScene_;
    QGraphicsView *graphicsView_;
    QVBoxLayout *layout_;
    QWidget *mainWidget_;
    std::unique_ptr<CoordsCalculator> calculator_;
    QToolBar *toolBar_;
    QPushButton *btn_;

    QMenu *mainMenu_;
    std::unique_ptr<std::thread> thread_;

    int bubblesCount_;
    unsigned int clickedIndex_;

    QPointF convertToScene(double x, double y);
    Point convertToLocal(const QPointF &point);

    std::shared_ptr<Locker> locker_;

public:
    explicit MainWindow( QWidget *parent = 0);
    ~MainWindow();

private slots:
    void sltStartStop();
    void sltReDraw();

    void sltBubbleMoved();
    void sltDeleteBubble();
    void sltBubbleClicked();

    void sltAddBubble(const QPointF& dst);

    void sltShowSettings();

    void sltCreateBubblesRandom();

    void sltCreateBubblesSquare();
    void sltCreateBubblesTriangle();
    void sltCreateBubblesHexagon();
};

#endif // MAINWINDOW_H
