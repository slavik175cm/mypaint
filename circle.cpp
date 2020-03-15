#include "circle.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMoveEvent>
#include "polygon.h"
Circle::Circle()
{
    zoom = 1;
    setAcceptHoverEvents(true);
}

QRectF Circle::boundingRect() const{
    return QRectF(x - radius, y - radius, radius * 2, radius * 2);
}

Circle* Circle::getfocused() {
    return Circle::focused;
}

void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    QPen pen; pen.setWidthF(2 / this->scale());
    painter->setPen(pen);
    painter->setBrush(Qt::blue);
    painter->drawEllipse(QPointF(x, y), radius, radius);

    painter->setBrush(Qt::yellow);
    painter->setPen(Qt::NoPen);
    painter->drawEllipse(QPointF(x, y), 3 / this->scale(), 3 / this->scale());
}

void Circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    qDebug() << 228;
    QPointF cursor = mapToScene(event->pos());
    x = cursor.x();
    y = cursor.y();

    this->setTransformOriginPoint(cursor);
    scene->update();
}

void Circle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Polygon::focused = NULL;
    Circle::focused = this;
}
