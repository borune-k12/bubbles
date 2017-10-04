#ifndef BUBBLEGRAPHICSITEM_H
#define BUBBLEGRAPHICSITEM_H

#include <QGraphicsObject>
#include <QGraphicsSceneDragDropEvent>
#include <QPainter>

class BubbleGraphicsItem : public QGraphicsObject
{
    Q_OBJECT

private:
    size_t index_;
    QPointF clickPoint_;
    qreal radius_;
    Qt::GlobalColor color_;

public:
    BubbleGraphicsItem(quint16 index, qreal radius, QPointF coords, Qt::GlobalColor color);
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

signals:
    void bubbleMoved(size_t index);
    void bubbleDeleted(size_t index);
};

#endif // BUBBLEGRAPHICSITEM_H
