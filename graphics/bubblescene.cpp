#include "bubblescene.h"
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>

BubbleScene::BubbleScene(QObject *parent) :
    QGraphicsScene(parent)
{

}

/**
 * @brief BubbleScene::mouseReleaseEvent событие клика на сцене
 * @param mouseEvent
 */
void BubbleScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    // проверяем, какая кнопка была нажата
    if(mouseEvent->button() == Qt::RightButton)
    {
        // если под курсором нет элементов, кидаем сигнал о том, что необходимо добавить шарик
        if(itemAt(mouseEvent->buttonDownScenePos(Qt::RightButton),QTransform()) == 0)
            emit addBubble(mouseEvent->buttonDownScenePos(Qt::RightButton));
    }

    // пробрасываем event дальше
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}
