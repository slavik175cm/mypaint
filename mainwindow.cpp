#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtDebug>
#include <QString>
#include <QtMath>
#include <QVector>

Polygon* Polygon::focused = NULL;
Circle* Circle::focused = NULL;
const double pi = 3.1415926535897932384626433832795;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Scene = new QGraphicsScene(this);
    Scene->setSceneRect(0,0,700,700);
    ui->graphicsView->setScene(Scene);
    //ui->graphicsView->setFrameStyle(0);
    ui->graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // off scroll bar
    ui->graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // off scroll bar
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setCacheMode(QGraphicsView::CacheNone);
    Scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    startTimer(10);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_add_polygon_clicked()
{
    Polygon *a = new Polygon();
    a->scene = Scene;
    a->draw();
    Scene->addItem(a);
    Scene->update();
}

void MainWindow::timerEvent(QTimerEvent *event) {
    if (Polygon::focused != NULL) {
        ui->square->setText(QString::number(Polygon::focused->square * pow(Polygon::focused->scale(), 2)));
        ui->perimeter->setText(QString::number(Polygon::focused->perimeter * Polygon::focused->scale()));

        if (Polygon::focused->iszooming)
            ui->zoom->setValue((Polygon::focused->scale() - 1) / 0.05 + 20);

        if (polygon_prevfocus != Polygon::focused) {
            ui->zoom_text->setText(QString::number(Polygon::focused->scale()));
            ui->zoom->setValue((Polygon::focused->scale() - 1) / 0.05 + 20);
            polygon_prevfocus = Polygon::focused;
            circle_prevfocus = NULL;
        }
    }
    if (Circle::focused != NULL) {
        if (Circle::focused->radius == -1) return;
        ui->square->setText(QString::number(pi * pow(Circle::focused->radius, 2) * pow(Circle::focused->scale(), 2)));
        ui->perimeter->setText(QString::number(pi * Circle::focused->radius * Circle::focused->scale()));
        if (Circle::focused->iszooming)
            ui->zoom->setValue((Circle::focused->scale() - 1) / 0.05 + 20);
        if (circle_prevfocus != Circle::focused) {
            ui->zoom_text->setText(QString::number(Circle::focused->scale()));
            ui->zoom->setValue((Circle::focused->scale() - 1) / 0.05 + 20);
            circle_prevfocus = Circle::focused;
            polygon_prevfocus = NULL;
        }
    }
}

void MainWindow::on_rotation_clicked()
{
    if (Polygon::focused != NULL) Polygon::focused->rotate(ui->lineEdit->text().toDouble());
}

void MainWindow::on_zoom_valueChanged(int value)
{
    double zoom = 1 + 0.05 * (value - 20);
    if (Polygon::focused != NULL) {
        if (Polygon::focused->iszooming) return;
        Polygon::focused->setScale(zoom);
        ui->zoom_text->setText(QString::number(Polygon::focused->scale()));
     }
    if (Circle::focused != NULL && !Circle::focused->iszooming) {
        Circle::focused->setScale(zoom);
        ui->zoom_text->setText(QString::number(Circle::focused->scale()));
        Scene->update();
    }
}

void MainWindow::on_change_zoom_button_clicked()
{
    bool ok;
    if (ui->zoom_text->text().toDouble(&ok) < 0) return;
    if (!ok) return;
    if (Polygon::focused != NULL)
        Polygon::focused->zoom_out(ui->zoom_text->text().toDouble());
    if (Circle::focused != NULL)
        Circle::focused->zoom_out(ui->zoom_text->text().toDouble());

}

QStringList MainWindow::myparse(QString text, bool *ok) {
    QStringList list = text.split("\n");
    QStringList normal_list;
    *ok = false;
    for (int i = 0; i < list.size(); i++)
        normal_list.append(list[i].split(" "));

    for (int i = 0; i < normal_list.size(); i++) {
        bool ok;
        normal_list[i].toDouble(&ok);
        if (!ok) return normal_list;
    }
    *ok = true;
    return normal_list;
}

void MainWindow::on_add_figure_clicked()
{
    bool ok;
    QStringList list = myparse(ui->polygon_coordinates->toPlainText(), &ok);
    if (!ok || list.size() % 2 != 0) return;
    Polygon *a = new Polygon();
    a->scene = Scene;
    Scene->addItem(a);
    for (int i = 0; i < list.size() - 1; i += 2)
        a->points.push_back(QPointF(list[i].toDouble(), list[i + 1].toDouble()));
    a->calc_center();
    a->calc_perimeter();
    a->setTransformOriginPoint(a->center);
    a->resize_bound();
    Polygon::focused = a;
    Scene->update();
    ui->polygon_coordinates->clear();
}

void MainWindow::on_add_circle_clicked()
{
    bool ok;
    QStringList list = myparse(ui->circle_coordinates->toPlainText(), &ok);
    if (!ok || list.size() % 3 != 0) return;
    for (int i = 0; i < list.size(); i += 3) {
        Circle *a = new Circle();
        a->x = list[i].toDouble();
        a->y = list[i + 1].toDouble();
        a->radius = list[i + 2].toDouble();
        a->setTransformOriginPoint(a->x, a->y);
        a->focused = a;
        a->bound = QRectF(a->x - a->radius, a->y - a->radius, a->radius * 2, a->radius * 2);
        a->scene = Scene;
        Scene->addItem(a);
    }
    Scene->update();
}

void MainWindow::on_draw_circle_clicked()
{
    Circle *a = new Circle();
    a->scene = Scene;
    Scene->addItem(a);
    a->draw();
    Scene->update();
}
