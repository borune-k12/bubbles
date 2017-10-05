#include "bubblegraphicsitem.h"
#include <QPainter>
#include <QGraphicsSceneDragDropEvent>

BubbleGraphicsItem::BubbleGraphicsItem(size_t index, qreal radius, QPointF coords, Qt::GlobalColor color, QGraphicsItem *parent)
    : QGraphicsObject(parent), index_(index), radius_(radius), color_(color)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    setSelected(false);
    setPos(coords);
}

QRectF BubbleGraphicsItem::boundingRect() const
{
    return QRectF(-radius_,-radius_,2*radius_,2*radius_);
}

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

QPointF BubbleGraphicsItem::getCenter() const
{
    return pos();
}

size_t BubbleGraphicsItem::getIndex() const
{
    return index_;
}

void BubbleGraphicsItem::updateIndex()
{
    index_--;
}

void BubbleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(clickPoint_ != pos())
            emit bubbleMoved();
        else emit bubbleDeleted();
    }

    // снимаем выделение с шарика
    setSelected(false);

    // пробрасываем event дальше
    QGraphicsObject::mouseReleaseEvent(event);
}

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



