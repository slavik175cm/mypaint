#ifndef POLYGON_H
#define POLYGON_H

#include <QObject>
#include <QGraphicsItem>
#include <QTimer>
#include <QTime>
#include <QElapsedTimer>

#include <QMainWindow>
class Polygon : public QGraphicsItem
{
public:
    Polygon();
    QRectF boundingRect() const;
    QRectF bound;
    QGraphicsScene *scene;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QVector<QPointF> points;
    bool isdrawing, ok, isrotate, clockwise, iszooming;
    QPointF cur, center;
    double angle, zoom, square, perimeter;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    double distance(QPointF a, QPointF b);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void draw();
    void resize_bound();
    static Polygon* focused;
    void calc_center();
    void rotate(double angle);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    QElapsedTimer time, time1;
    void zoom_out(double zoom);
    void calc_perimeter();
};
//Polygon* Polygon::focused = NULL;

#endif // POLYGON_H
