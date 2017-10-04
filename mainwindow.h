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
#include <QMenu>

class BubbleScene;

#define BUBBLE_RADIUS 15
#define DEFAULT_BUBBLES_COUNT 5

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    ThreadSafeVector<Bubble> *bubbles_;
    BubbleScene *graphicsScene_;
    QGraphicsView *graphicsView_;
    QVBoxLayout *layout_;
    QWidget *mainWidget_;
    CoordsCalculator *calculator_;
    QToolBar *toolBar_;
    QPushButton *btn_;

    QMenu *mainMenu_;
    std::thread *thread_;

    int bubblesCount_;

public:
    MainWindow( QWidget *parent = 0);
    ~MainWindow();

private slots:
    void start();
    void redraw();

    void bubbleMoved(size_t index);
    void bubbleDeleted(size_t index);
    void addBubble(const QPointF& dst);

    void showSettings();

    void _createBubbles();

};

#endif // MAINWINDOW_H
