#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QtDebug>
#include <QString>
Polygon* Polygon::focused = NULL;

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
    if (Polygon::getfocused() == NULL) return;
    if (prevfocus != Polygon::getfocused()) {
        ui->zoom_text->setText(QString::number(Polygon::getfocused()->scale()));
        ui->zoom->setValue((Polygon::getfocused()->scale() - 1) / 0.05 + 20);
        prevfocus = Polygon::getfocused();
    }
}

void MainWindow::on_rotation_clicked()
{
    if (Polygon::getfocused() != NULL) Polygon::getfocused()->rotate(ui->lineEdit->text().toDouble());
}

void MainWindow::on_zoom_valueChanged(int value)
{
    double zoom = 1 + 0.05 * (value - 20);
    if (Polygon::getfocused() != NULL) {
        Polygon::getfocused()->setScale(zoom);
        ui->zoom_text->setText(QString::number(Polygon::getfocused()->scale()));
     }
}

void MainWindow::on_pushButton_clicked()
{
    if (Polygon::getfocused() != NULL) Polygon::getfocused()->zoom_out(ui->zoom_text->text().toDouble());
}

