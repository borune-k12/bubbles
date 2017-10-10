#ifndef BUBBLEGRAPHICSITEM_H
#define BUBBLEGRAPHICSITEM_H

#include <QGraphicsObject>

class QGraphicsSceneDragDropEvent;
class QPainter;

/**
 * @brief The BubbleGraphicsItem class класс для представления шарика на сцене
 */
class BubbleGraphicsItem : public QGraphicsObject
{
    Q_OBJECT

private:
    int index_;             // индекс шарика
    QPointF clickPoint_;    // координаты точки клика
    qreal radius_;          // радиус шарика
    Qt::GlobalColor color_; // цвет шарика

public:
    BubbleGraphicsItem(int index, qreal radius, QPointF coords, Qt::GlobalColor color, QGraphicsItem * parent = 0);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = Q_NULLPTR);

    QPointF getCenter() const;
    int getIndex() const;
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
