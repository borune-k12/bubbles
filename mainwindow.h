#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "coordscalculator.h"
#include <thread>
#include <memory>


#include "locker.h"

class QGraphicsScene;
class QGraphicsView;
class QVBoxLayout;
class QPushButton;
class QMenu;
class BubbleScene;
class BubbleGraphicsItem;

#define DEFAULT_BUBBLES_COUNT 5

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

    void sltCreateBubbles();

};

#endif // MAINWINDOW_H
