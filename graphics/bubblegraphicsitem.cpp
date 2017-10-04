#include "bubblegraphicsitem.h"

BubbleGraphicsItem::BubbleGraphicsItem(quint16 index, qreal radius, QPointF coords, Qt::GlobalColor color)
    : QGraphicsObject(), index_(index), radius_(radius), color_(color)
{
    setFlags(QGraphicsItem::ItemIsMovable);
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

    QRadialGradient radialGrad(boundingRect().center(), radius_);
    radialGrad.setColorAt(0, Qt::white);
    radialGrad.setColorAt(0.6, color_);
    radialGrad.setColorAt(1, color_);
    painter->setBrush(radialGrad);

    painter->setPen(QPen(color_));
    painter->drawEllipse(boundingRect());
}

void BubbleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        if(clickPoint_ != mapToScene(event->pos()))
            emit bubbleMoved(index_);
    }
    QGraphicsObject::mouseReleaseEvent(event);
}

void BubbleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        clickPoint_ = mapToScene(event->pos());
        emit bubbleDeleted(index_);
    }

    QGraphicsObject::mousePressEvent(event);
}



