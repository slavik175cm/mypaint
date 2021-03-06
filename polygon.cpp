#include "polygon.h"
#include <QPainter>
#include <qdebug.h>
#include "QGraphicsScene"
#include "QVector"
#include "QGraphicsSceneMouseEvent"
#include <QTimer>
#include <QThread>
#include <QList>
#include <QTime>
#include <QTimerEvent>
#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtMath>
#include "circle.h"
Polygon::Polygon()
{
    isdrawing = ok = 0;
    cur = QPointF(-1, -1);
    center = QPointF(0, 0);
    setAcceptHoverEvents(true);
    points.resize(0);
    bound = QRectF(0, 0, 0, 0);
    isrotate = 0;
    angle = 0;
    iszooming = 0;
    square = 0;
}

QRectF Polygon::boundingRect() const
{
    return bound;
}

void Polygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    if (iszooming) {
        bool ok;
        if (zoom > this->scale()) ok = 1; else ok = 0;
        if (ok) this->setScale(this->scale() +  0.001 * time1.elapsed());
            else this->setScale(this->scale() - 0.001 * time1.elapsed());
        if ((ok && zoom <= this->scale()) || (!ok && zoom >= this->scale())) {
            this->setScale(zoom);
            iszooming = 0;
        }
        if (time1.elapsed() != 0) time1.restart();
    }
    if (isrotate) {
        if (clockwise) this->setRotation(this->rotation() + 0.1 * time.elapsed());
            else this->setRotation(this->rotation() - 0.1 * time.elapsed());

        if (time.elapsed() != 0) time.restart();
        if ((clockwise && this->rotation() >= angle) || (!clockwise && this->rotation() <= angle))
            isrotate = 0;
    }
    if (isdrawing) {
        for (int i = 0; i < points.size() - 1; i++) {
            painter->drawLine(points[i], points[i + 1]);
        }
        if (cur.x() != -1 && points.size() > 0) painter->drawLine(points[points.size() - 1], cur);
    } else {
        QPen pen;
        pen.setWidthF(2 / this->scale());
        painter->setPen(pen);
        painter->setBrush(QBrush(Qt::blue));
        painter->drawPolygon(points);
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(Qt::yellow));
        painter->drawEllipse(center, 2 / this->scale(), 3 / this->scale());
    }
}

void Polygon::draw() {
    isdrawing = 1;
    bound = QRectF(0, 0, 700, 700);
}

double Polygon::distance(QPointF a, QPointF b) {
    return sqrt((a.x() - b.x()) * (a.x() - b.x()) + (a.y() - b.y()) * (a.y() - b.y()));
}

void Polygon::resize_bound() {
    double mnx = 700, mxx = 0, mny = 700, mxy = 0;
    for (int i = 0; i < points.size(); i++) {
        mnx = qMin(mnx, points[i].x());
        mny = qMin(mny, points[i].y());
        mxx = qMax(mxx, points[i].x());
        mxy = qMax(mxy, points[i].y());
    }
    bound = QRectF(mnx, mny, mxx - mnx, mxy - mny);
}

void Polygon::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    Circle::focused = NULL;
    focused = this;
    if (!isdrawing) return;
    points.push_back(event->pos());
    if (points.size() != 1 && distance(points[0], points[points.size() - 1]) < 10) {
        isdrawing = 0;
        points.pop_back();
        calc_center();
        calc_perimeter();
        this->setTransformOriginPoint(center);
        resize_bound();
    }
    scene->update();
}

void Polygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event) {
    if (isdrawing) {
        if (points.size() != 0) {
            cur = event->pos();
            scene->update();
        }
    }
}

void Polygon::calc_center()
{
    double BackX, BackY, NextX, NextY;
    center.setX(0); center.setY(0);
    for(int i = 0; i < points.size(); i++)
    {
        BackX = points[i].x();
        BackY = points[i].y();
        NextY = points[(i + 1) % points.size()].y();
        NextX = points[(i + 1) % points.size()].x();
        double a = BackX * NextY - NextX * BackY;
        square += a;
        center.setX(center.x() + (BackX + NextX) * a);
        center.setY(center.y() + (BackY + NextY) * a);
    }
    square /= 2;
    if (square == 0) return;
    center.setX(center.x() / (square * 6));
    center.setY(center.y() / (square * 6));
    square = abs(square);
}

void Polygon::rotate(double angle) {
    if (isdrawing) return;
    this->angle = this->rotation() + angle;
    if (angle > 0) clockwise = 1; else clockwise = 0;
    this->setTransformOriginPoint(center);
    isrotate = 1;
    time.restart();
}

void Polygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    QPointF cursor = mapToScene(event->pos());
    if (isdrawing) return;
    for (int i = 0; i < points.size(); i++) {
        points[i].setX(points[i].x() - center.x() + cursor.x());
        points[i].setY(points[i].y() - center.y() + cursor.y());
    }
    center = cursor;
    this->setTransformOriginPoint(center);
    resize_bound();
    scene->update();
}

void Polygon::zoom_out(double zoom) {
    iszooming = 1;
    this->zoom = zoom;
    time1.restart();
    scene->update();
}

void Polygon::calc_perimeter() {
    double perimeter = 0;
    for (int i = 0; i < points.size() - 1; i++) {
        perimeter += distance(points[i], points[i + 1]);
    }
    perimeter += distance(points[points.size() - 1], points[0]);
    this->perimeter = perimeter;
}
