#ifndef BUUBLESCENE_H
#define BUUBLESCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class BubbleScene : public QGraphicsScene
{

    Q_OBJECT

public:
    BubbleScene();

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void addBubble(const QPointF& dst);

};

#endif // BUUBLESCENE_H
