#include <QtGui>
#include <QtNetwork>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "downloadimage.h"
#include "cmap.h"
#include "cmapwidget.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mapWidget = new CMapWidget(this);
    //setCentralWidget(mapWidget);
    //vlayout = new QVBoxLayout();
    //vlayout->set
    //vlayout->addWidget(mapWidget);
    //ui->centralWidget->setLayout((vlayout));
    ui->verticalLayout->addWidget(mapWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Draw_map_triggered()
{
    mapWidget->setCenterMap(21.033333, 105.849998);
    //myMap->
}


void MainWindow::on_actionVien_CNTT_triggered()
{
    mapWidget->setCenterMap(21.046672, 105.801577);
}

void MainWindow::on_actionDownload_From_URL_triggered()
{

}
