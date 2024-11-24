#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new CustomScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,500,500);
}

MainWindow::~MainWindow()
{
    delete ui;
}
