#ifndef BUUBLESCENE_H
#define BUUBLESCENE_H

#include <QGraphicsScene>

class QGraphicsSceneMouseEvent;

/** класс сцены */
class BubbleScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit BubbleScene(QObject *parent = 0);

protected:
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);

signals:
    void addBubble(const QPointF& dst);

};

#endif // BUUBLESCENE_H
