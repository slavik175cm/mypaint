#ifndef CIRCLE_H
#define CIRCLE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
class Circle : public QGraphicsItem
{
public:
    double x, y, radius, zoom;
    Circle();
    QRectF boundingRect() const;
    QRectF bound;
    static Circle* focused;
    static Circle* getfocused();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    QGraphicsScene *scene;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // CIRCLE_H
