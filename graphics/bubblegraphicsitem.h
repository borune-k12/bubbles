#ifndef BUBBLEGRAPHICSITEM_H
#define BUBBLEGRAPHICSITEM_H

#include <QGraphicsObject>

class QGraphicsSceneDragDropEvent;
class QPainter;

/** класс для представления шарика на сцене */

class BubbleGraphicsItem : public QGraphicsObject
{
    Q_OBJECT

private:
    size_t index_;          // индекс шарика
    QPointF clickPoint_;    // координаты точки клика
    qreal radius_;          // радиус шарика
    Qt::GlobalColor color_; // цвет шарика

public:
    BubbleGraphicsItem(size_t index, qreal radius, QPointF coords, Qt::GlobalColor color, QGraphicsItem * parent = 0);

    // переопределенные методы от QGraphicsItem
    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    // получить центр шарика
    QPointF getCenter() const;

    // получить индекс шарика
    size_t getIndex() const;

    // пересчитать индекс
    void updateIndex();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

signals:
    void bubbleMoved();
    void bubbleDeleted();
    void bubbleClicked();
};

#endif // BUBBLEGRAPHICSITEM_H
