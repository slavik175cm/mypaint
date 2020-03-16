#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QElapsedTimer>
class Circle : public QGraphicsItem
{
public:
    double x, y, radius, zoom;
    bool iszooming, isdrawing;
    QElapsedTimer time;
    Circle();
    QPoint realcenter;
    QRectF boundingRect() const;
    void zoom_out(double zoom);
    QRectF bound;
    QPointF cur;
    static Circle* focused;
    static Circle* getfocused();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    QGraphicsScene *scene;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    double distance(QPointF a, QPointF b);
    void draw();
};

#endif // CIRCLE_H
