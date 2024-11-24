#include "customscene.h"

int radius = 50;

CustomScene::CustomScene(QObject *parent): QGraphicsScene(parent) {}

void CustomScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(items().isEmpty()){
        QGraphicsPolygonItem *item = new QGraphicsPolygonItem(drawCircle(event->scenePos()));
        item->setPen(QPen(Qt::cyan, 2));
        addItem(item);
    }
    else {
        mouseHandleEvent(event);
    }
    QGraphicsScene::mousePressEvent(event);
}

void CustomScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    mouseHandleEvent(event);
    QGraphicsScene::mouseMoveEvent(event);
}

QPolygonF CustomScene::drawCircle(const QPointF &pos)
{
    QPolygonF circle;
    for (int i = 0; i < 360; ++i) {
        qreal angle = qDegreesToRadians(static_cast<int>(i));
        circle << QPointF(pos.x() + radius * qCos(angle), pos.y() + radius * qSin(angle));
    }
    return circle;
}

void CustomScene::mergeItems(QGraphicsPolygonItem *polyItem)
{
    for (QGraphicsItem *item : items()) {
        if (item != polyItem) {
            QGraphicsPolygonItem *polyItem2 = dynamic_cast<QGraphicsPolygonItem *>(item);
            if (polyItem2) {
                if (polyItem->collidesWithItem(polyItem2)) {
                    polyItem->setPolygon(polyItem->polygon().united(polyItem2->polygon()));
                    removeItem(polyItem2);
                }
            } else {
                qDebug() << "Error: Unexpected item type in mergeItem.";
            }
        }
    }
}

void CustomScene::splitItem(QGraphicsPolygonItem *polyItem, const QPointF &pos)
{
    QPolygonF leftPoly, rightPoly, upperPoly, lowerPoly;
    for (const QPointF &point : polyItem->polygon()) {
        if (point.x() < pos.x()) {
            leftPoly << point;
        } else {
            rightPoly << point;
        }
        if (point.y() < pos.y()) {
            upperPoly << point;
        } else {
            lowerPoly << point;
        }
    }
    double minDistanceX = 10000;
    for (const QPointF &p1 : leftPoly) {
        for (const QPointF &p2 : rightPoly) {
            double distance = qSqrt(qPow(p2.x() - p1.x(), 2) + qPow(p2.y() - p1.y(), 2));
            if (distance < minDistanceX) {
                minDistanceX = distance;
            }
        }
    }
    double minDistanceY = 10000;
    for (const QPointF &p1 : upperPoly) {
        for (const QPointF &p2 : lowerPoly) {
            double distance = qSqrt(qPow(p2.x() - p1.x(), 2) + qPow(p2.y() - p1.y(), 2));
            if (distance < minDistanceY) {
                minDistanceY = distance;
            }
        }
    }
    if (minDistanceX > 1) {
        upperPoly.clear();
        lowerPoly.clear();
        polyItem->setPolygon(leftPoly);
        QGraphicsPolygonItem *item2 = new QGraphicsPolygonItem(rightPoly);
        item2->setPen(QPen(Qt::cyan, 2));
        addItem(item2);
    } else if (minDistanceY > 1) {
        leftPoly.clear();
        rightPoly.clear();
        polyItem->setPolygon(upperPoly);
        QGraphicsPolygonItem *item2 = new QGraphicsPolygonItem(lowerPoly);
        item2->setPen(QPen(Qt::cyan, 2));
        addItem(item2);
    }
}

void CustomScene::mouseHandleEvent(QGraphicsSceneMouseEvent *event)
{
    if (!items().isEmpty()) {
        for (QGraphicsItem *item : items()) {
            QGraphicsPolygonItem *polyItem = dynamic_cast<QGraphicsPolygonItem *>(item);
            if (polyItem) {
                if (item->contains(event->scenePos())) {
                    polyItem->setPolygon(polyItem->polygon().united(drawCircle(event->scenePos())));
                    mergeItems(polyItem);
                } else {
                    polyItem->setPolygon(polyItem->polygon().subtracted(drawCircle(event->scenePos())));
                    if (polyItem->polygon().isEmpty()) {
                        removeItem(item);
                    } else {
                        splitItem(polyItem, event->scenePos());
                    }
                }
            } else {
                qDebug() << "Error: Unexpected item";
            }
        }
    }
}
