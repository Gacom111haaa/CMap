#include <QtGui>
#include <QtNetwork>

#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "Lightmap.h"
//#include "SlippyMap.h"
#include "downloadimage.h"
#include "cmap.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    myMap=new CMap(this);
    myMap->setCenterPos(21.036264,105.774866);
    myMap->setScaleRatio(15);
    myMap->setWidthHeight(400,300);
    myMap->setPath("C:/Users/LamPT/Desktop/mapData");
    myMap->invalidate();

    //connect(myMap, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_Draw_map_triggered()
{
    setCenterMap(21.033333, 105.849998);
    //myMap->
}

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    myMap->render(&p, e->rect());

    p.setPen((Qt::black));
    p.drawText(rect(), Qt::AlignBottom|Qt::TextWordWrap,
               "Day la ham thu nghiem cua LamPT");
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    //myMap->width = width();
    //myMap->height = height();
    myMap->setWidthHeight(width(), height());
    myMap->invalidate();
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() != Qt::LeftButton)
    {
        return;
    }

    pressed = snapped = true;
    pressPos = dragPos = e->pos();
    //tapTimer.stop();
    //tapTimer.start(HOLD_TIME, this);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
    if (!e->buttons())
        return;

    //if (!zoomed) {
        if (!pressed || !snapped) {
            QPoint delta = e->pos() - pressPos;
            pressPos = e->pos();
            myMap->pan(delta);
            return;
        } else {
            const int threshold = 10;
            QPoint delta = e->pos() - pressPos;
            if (snapped) {
                snapped &= delta.x() < threshold;
                snapped &= delta.y() < threshold;
                snapped &= delta.x() > -threshold;
                snapped &= delta.y() > -threshold;
            }
       //     if (!snapped)
       //         tapTimer.stop();
       }
       // } else {
            dragPos = e->pos();
            update();
        //}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    update();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
     if (e->key() != Qt::Key_Z )
     {
         return;
     }
     else{
         int zoom = (int) myMap->getScaleRatio();
         switch(zoom)
         {
         case 14: myMap->setScaleRatio(15);
             break;
         case 15: myMap->setScaleRatio(16);
             break;
         case 16: myMap->setScaleRatio(14);
             break;
         default: myMap->setScaleRatio(15);
             break;
         }

         myMap->invalidate();
         update();
     }
}

void MainWindow::setCenterMap(qreal mlat, qreal mlong)
{
    //myMap->latitude = mlat;
    //myMap->longitude = mlong;
    myMap->setCenterPos(mlat, mlong);
    myMap->invalidate();
}

void MainWindow::sessionOpened()
{
    // Save the used configuration
    QNetworkConfiguration config = networkSession->configuration();
    QString id;
    if (config.type() == QNetworkConfiguration::UserChoice) {
        id = networkSession->sessionProperty(
                QLatin1String("UserChoiceConfiguration")).toString();
    } else {
        id = config.identifier();
    }

    QSettings settings(QSettings::UserScope, QLatin1String("Trolltech"));
    settings.beginGroup(QLatin1String("QtNetwork"));
    settings.setValue(QLatin1String("DefaultNetworkConfiguration"), id);
    settings.endGroup();
}

void MainWindow::on_actionVien_CNTT_triggered()
{
    setCenterMap(21.046672, 105.801577);
}

void MainWindow::on_actionDownload_From_URL_triggered()
{

}
