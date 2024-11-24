#ifndef CUSTOMSCENE_H
#define CUSTOMSCENE_H

#include <QtMath>
#include <QDebug>
#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

extern int radius;

class CustomScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit CustomScene(QObject *parent = nullptr);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QPolygonF drawCircle(const QPointF &pos);

    void mergeItems(QGraphicsPolygonItem *polyItem);
    void mouseHandleEvent(QGraphicsSceneMouseEvent *event);
    void splitItem(QGraphicsPolygonItem *polyItem, const QPointF &pos);
};

#endif // CUSTOMSCENE_H
