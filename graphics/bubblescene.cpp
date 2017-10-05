#include "bubblescene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

BubbleScene::BubbleScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

void BubbleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)
    {
        // если под курсором нет элементов, кидаем сигнал
        if(itemAt(mouseEvent->buttonDownScenePos(Qt::RightButton),QTransform()) == 0)
            emit addBubble(mouseEvent->buttonDownScenePos(Qt::RightButton));
    }

    // пробрасываем event дальше
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
