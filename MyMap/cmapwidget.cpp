#include "cmapwidget.h"
#include "cmap.h"

CMapWidget::CMapWidget(QWidget *parent) : QWidget(parent)
{
    myMap = new CMap(this);
    myMap->setCenterPos(21.036264,105.774866);
    myMap->setScaleRatio(15);
    myMap->setWidthHeight(400,300);
    myMap->setPath("C:/Users/LamPT/Desktop/mapData");
    myMap->invalidate();
}

void CMapWidget::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    myMap->render(&p, e->rect());

    p.setPen((Qt::black));
    p.drawText(rect(), Qt::AlignBottom|Qt::TextWordWrap,
               "Day la ham thu nghiem cua LamPT");
}

void CMapWidget::resizeEvent(QResizeEvent *)
{
    //myMap->width = width();
    //myMap->height = height();
    myMap->setWidthHeight(width(), height());
    myMap->invalidate();
}

void CMapWidget::mousePressEvent(QMouseEvent *e)
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

void CMapWidget::mouseMoveEvent(QMouseEvent *e)
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

void CMapWidget::mouseReleaseEvent(QMouseEvent *)
{
    update();
}

void CMapWidget::keyPressEvent(QKeyEvent *e)
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

void CMapWidget::setCenterMap(qreal mlat, qreal mlong)
{
    myMap->setCenterPos(mlat, mlong);
    myMap->invalidate();
}

