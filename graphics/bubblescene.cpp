#include "bubblescene.h"
#include <QGraphicsItem>

BubbleScene::BubbleScene() :
    QGraphicsScene()
{

}

void BubbleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::RightButton)
    {
        //QPointF clickPoint = mouseEvent->buttonDownScenePos(Qt::RightButton);
        if(itemAt(mouseEvent->buttonDownScenePos(Qt::RightButton),QTransform()) == 0)
            emit addBubble(mouseEvent->buttonDownScenePos(Qt::RightButton));
    }

    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
