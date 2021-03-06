#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "polygon.h"
#include "circle.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QGraphicsScene *Scene;
    void timerEvent(QTimerEvent *event);
    Polygon *polygon_prevfocus = NULL;
    Circle *circle_prevfocus = NULL;
    QStringList myparse(QString text, bool *ok);
private slots:
    void on_add_polygon_clicked();

    void on_rotation_clicked();


    void on_zoom_valueChanged(int value);

    void on_change_zoom_button_clicked();

    void on_add_figure_clicked();

    void on_add_circle_clicked();

    void on_draw_circle_clicked();

    void keyPressEvent(QKeyEvent *event);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
