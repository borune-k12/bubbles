#include "bubblegraphicsitem.h"
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

/**
 * @brief BubbleGraphicsItem::BubbleGraphicsItem
 * @param index индекс шарика в векторе
 * @param radius радиус шарика
 * @param coords координаты шарика
 * @param color цвет шарика
 * @param parent указатель на родителя
 */
BubbleGraphicsItem::BubbleGraphicsItem(int index, qreal radius, QPointF coords, Qt::GlobalColor color, QGraphicsItem *parent)
    : QGraphicsObject(parent), index_(index), radius_(radius), color_(color)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setSelected(false);
    setPos(coords);
}

/**
 * @brief BubbleGraphicsItem::boundingRect вернуть boundingRect шарика
 * @return
 */
QRectF BubbleGraphicsItem::boundingRect() const
{
    return QRectF(-radius_,-radius_,2*radius_,2*radius_);
}

/**
 * @brief BubbleGraphicsItem::paint отрисовка шарика
 * @param painter
 * @param option
 * @param widget
 */
void BubbleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    // рисуем красивый шарик
    QRadialGradient radialGrad(boundingRect().center(), radius_);
    radialGrad.setColorAt(0, Qt::white);
    radialGrad.setColorAt(0.6, color_);
    radialGrad.setColorAt(1, color_);
    painter->setBrush(radialGrad);

    // задаем цвет линии в зависимости от состояния
    QPen pen(isSelected()?Qt::green:color_);
    pen.setWidth(isSelected()?3:1);
    painter->setPen(pen);

    // рисуем шарик
    painter->drawEllipse(boundingRect());
}

/**
 * @brief BubbleGraphicsItem::getCenter вернуть координаты центра шарика
 * @return центр шарика
 */
QPointF BubbleGraphicsItem::getCenter() const
{
    return pos();
}

/**
 * @brief BubbleGraphicsItem::getIndex вернуть индекс шарика
 * @return индекс шарика
 */
int BubbleGraphicsItem::getIndex() const
{
    return index_;
}

/**
 * @brief BubbleGraphicsItem::updateIndex обновить индекс (после удаления шарика из вектора)
 */
void BubbleGraphicsItem::updateIndex()
{
    index_--;
}

/**
 * @brief BubbleGraphicsItem::mouseReleaseEvent событие отпускания кнопки мыши
 * @param event
 */
void BubbleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    // проверяем, какая кнопка была нажата
    if(event->button() == Qt::LeftButton)
    {
        // если курсор был перемещен, значит двигаем шарик
        if(clickPoint_ != pos())
            emit bubbleMoved();
        // иначе лопаем
        else emit bubbleDeleted();
    }

    // снимаем выделение с шарика
    setSelected(false);

    // пробрасываем event дальше
    QGraphicsObject::mouseReleaseEvent(event);
}

/**
 * @brief BubbleGraphicsItem::mousePressEvent событие клика на шарике
 * @param event
 */
void BubbleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        // сохраняем координаты клика
        clickPoint_ = pos();
        emit bubbleClicked();
    }    

    // выделяем шарик
    setSelected(true);

    // пробрасываем event дальше
    QGraphicsObject::mousePressEvent(event);
}



