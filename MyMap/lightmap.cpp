#include "Lightmap.h"
#include "SlippyMap.h"

#include <QtGui>
#include <QtNetwork>

Lightmap::Lightmap(QWidget *parent):QWidget(parent)
{
    m_myMap = new SlippyMap(this);
    connect(m_myMap, SIGNAL(updated(QRect)), SLOT(updateMap(QRect)));
}

void Lightmap::paintEvent(QPaintEvent *event)
{
    QPainter p;
    p.begin(this);
    m_myMap->render(&p, event->rect());

    p.setPen((Qt::black));
    p.drawText(rect(), Qt::AlignBottom|Qt::TextWordWrap,
               "Day la ham thu nghiem cua LamPT");

}

void Lightmap::setCenter(qreal lat, qreal lng)
{
 m_myMap->latitude = lat;
 m_myMap->longitude = lng;
}

void Lightmap::updateMap(const QRect &r)
{
    update(r);
}
