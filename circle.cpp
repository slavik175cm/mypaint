#include "circle.h"
#include <QPainter>
#include <QDebug>
#include <QGraphicsSceneMoveEvent>
#include <QGraphicsSceneHoverEvent>
#include "polygon.h"
#include <QtMath>
#include <QApplication>
#include <QScreen>
Circle::Circle()
{
    radius = -1;
    zoom = 1;
    iszooming = 0;
    isdrawing = 0;
    x = y = -1;
    setAcceptHoverEvents(true);
}

QRectF Circle::boundingRect() const{
    return bound;
}

double Circle::distance(QPointF a, QPointF b) {
    return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}


void Circle::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (iszooming) {
        bool ok;
        if (this->scale() < zoom) ok = 1; else ok = 0;
        this->setScale(this->scale() + 0.002 * time.elapsed() * (ok ? 1 : -1));
        if ((ok && this->scale() >= zoom) || (!ok && this->scale() <= zoom))
            iszooming = 0;
        if (time.elapsed() != 0) time.restart();
    }
    if (isdrawing && x != -1) {
        double dist= distance(QPointF(x, y), cur);
        painter->drawEllipse(QPointF(x, y), dist, dist);

        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(x, y), 3, 3);
    } else {
        QPen pen; pen.setWidthF(2 / this->scale());
        painter->setPen(pen);
        painter->setBrush(Qt::blue);
        painter->drawEllipse(QPointF(x, y), radius, radius);

        painter->setBrush(Qt::yellow);
        painter->setPen(Qt::NoPen);
        painter->drawEllipse(QPointF(x, y), 3 / this->scale(), 3 / this->scale());
    }
}

void Circle::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF cursor = mapToScene(event->pos());
    x = cursor.x();
    y = cursor.y();
    bound = QRectF(x - radius, y - radius, radius * 2, radius * 2);
    this->setTransformOriginPoint(cursor);
    scene->update();
}

void Circle::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Polygon::focused = NULL;
    Circle::focused = this;
    if (isdrawing) {
        if (x == -1) {
            x = event->pos().x();
            y = event->pos().y();
            QCursor cursor(Qt::BlankCursor);
            realcenter = cursor.pos();
            QApplication::setOverrideCursor(cursor);
            QApplication::changeOverrideCursor(cursor);
        } else {
            radius = distance(QPointF(x, y), event->pos());
            isdrawing = 0;
            bound = QRectF(x - radius, y - radius, radius * 2, radius * 2);
            QCursor cursor(Qt::CustomCursor);
            cursor.setPos(realcenter);
            QApplication::setOverrideCursor(cursor);
            QApplication::changeOverrideCursor(cursor);
            this->setTransformOriginPoint(QPointF(x, y));
        }
    }
}

void Circle::zoom_out(double zoom) {
    iszooming = 1;
    this->zoom = zoom;
    time.restart();
    scene->update();
}

void Circle::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    if (isdrawing) {
        if (radius == -1) {
            cur = event->pos();
            scene->update();
        }
    }
}

void Circle::draw() {
    isdrawing = 1;
    bound = QRectF(0, 0, 700, 700);
    scene->update();
}

