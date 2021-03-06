/****************************************************************************
**
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the demonstration applications of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <math.h>

#include <QtGui>
#include <QtNetwork>
#include "SlippyMap.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

uint qHash(const QPoint& p)
{
    return p.x() * 17 ^ p.y();
}

// tile size in pixels
const int tdim = 256;

// xac dinh manh ban do tu toa do
QPointF tileForCoordinate(qreal lat, qreal lng, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal tx = (lng + 180.0) / 360.0;
    qreal ty = (1.0 - log(tan(lat * M_PI / 180.0) +
                          1.0 / cos(lat * M_PI / 180.0)) / M_PI) / 2.0;
    return QPointF(tx * zn, ty * zn);
}


//tra ve kinh do tu manh ban do
qreal longitudeFromTile(qreal tx, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal lat = tx / zn * 360.0 - 180.0;
    return lat;
}

//tra ve vi do tu manh ban do
qreal latitudeFromTile(qreal ty, int zoom)
{
    qreal zn = static_cast<qreal>(1 << zoom);
    qreal n = M_PI - 2 * M_PI * ty / zn;
    qreal lng = 180.0 / M_PI * atan(0.5 * (exp(n) - exp(-n)));
    return lng;
}


//ham khoi tao, rong 400, cao 300, zoom o muc 15, voi toa do co dinh
SlippyMap::SlippyMap(QObject *parent): QObject(parent)
{
    //khoi tao mot vung pixmap co kich thuoc 256 X 256 va boi day no voi mau lightGray
    m_emptyTile = QPixmap(tdim, tdim);
    m_emptyTile.fill(Qt::lightGray);

    //ket noi mang
    QNetworkDiskCache *cache = new QNetworkDiskCache;
    cache->setCacheDirectory(QDesktopServices::storageLocation
                                (QDesktopServices::CacheLocation));
    m_manager.setCache(cache);
    //connect(&m_manager, SIGNAL(finished(QNetworkReply*)),
    //        this, SLOT(handleNetworkData(QNetworkReply*)));
}

//lam mat hieu luc? -> chac la kieu reload
void SlippyMap::invalidate()
{
    if (width <= 0 || height <= 0)
        return;

    // xac dinh diem ban do
    QPointF ct = tileForCoordinate(latitude, longitude, zoom);
    qreal tx = ct.x();
    qreal ty = ct.y();


    // top-left corner of the center tile
    //diem tren cung ben trai cua manh ban do trung tam
    int xp = width / 2 - (tx - floor(tx)) * tdim;
    int yp = height / 2 - (ty - floor(ty)) * tdim;

    // first tile vertical and horizontal
    // manh ban do dau tien theo chieu doc va ngang
    int xa = (xp + tdim - 1) / tdim;
    int ya = (yp + tdim - 1) / tdim;
    int xs = static_cast<int>(tx) - xa;
    int ys = static_cast<int>(ty) - ya;

    // offset for top-left tile
    // offset cho manh tren cung ben trai
    m_offset = QPoint(xp - xa * tdim, yp - ya * tdim);

    // last tile vertical and horizontal
    // manh cuoi cung doc va ngang
    int xe = static_cast<int>(tx) + (width - xp - 1) / tdim;
    int ye = static_cast<int>(ty) + (height - yp - 1) / tdim;

    // build a rect
    // xay dung ban do
    m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);

    //neu duong link ko rong thi thuc hien dơnload
    if (m_url.isEmpty())
        LoadMap();//download();

    emit updated(QRect(0, 0, width, height));
}

//ham viet du lieu hien thi tren su kien Painter
void SlippyMap::render(QPainter *p, const QRect &rect)
{
    for (int x = 0; x <= m_tilesRect.width(); ++x)
        for (int y = 0; y <= m_tilesRect.height(); ++y) {
            QPoint tp(x + m_tilesRect.left(), y + m_tilesRect.top());
            QRect box = tileRect(tp);
            if (rect.intersects(box)) {
                if (m_tilePixmaps.contains(tp))
                    p->drawPixmap(box, m_tilePixmaps.value(tp));
                else
                    p->drawPixmap(box, m_emptyTile);
            }
        }
}

//hien chua dung, chac dung cho chuc nang khac
void SlippyMap::pan(const QPoint &delta)
{
    QPointF dx = QPointF(delta) / qreal(tdim);
    QPointF center = tileForCoordinate(latitude, longitude, zoom) - dx;
    latitude = latitudeFromTile(center.y(), zoom);
    longitude = longitudeFromTile(center.x(), zoom);
    invalidate();
}


//ham nay download du lieu tren mang xuong day
void SlippyMap::handleNetworkData(QNetworkReply *reply)
{
    QImage img;
    QPoint tp = reply->request().attribute(QNetworkRequest::User).toPoint();
    QUrl url = reply->url();
    if (!reply->error())
        if (!img.load(reply, 0))
            img = QImage();
    reply->deleteLater();

    m_tilePixmaps[tp] = QPixmap::fromImage(img);
    if (img.isNull())
        m_tilePixmaps[tp] = m_emptyTile;
    emit updated(tileRect(tp));

    // purge unused spaces
    QRect bound = m_tilesRect.adjusted(-2, -2, 2, 2);
    foreach(QPoint tp, m_tilePixmaps.keys())
    if (!bound.contains(tp))
        m_tilePixmaps.remove(tp);

    download();
}

//
void SlippyMap::LoadMap()
{
    QPoint grab(0, 0);
    for (int x = 0; x <= m_tilesRect.width(); ++x)
        for (int y = 0; y <= m_tilesRect.height(); ++y) {
            QPoint tp = m_tilesRect.topLeft() + QPoint(x, y);
            if (!m_tilePixmaps.contains(tp)) {
                grab = tp;
                break;
            }
        }
    if (grab == QPoint(0, 0)) {
        m_url = QUrl();
        return;
    }


    QString path = "C:/Users/LamPT/Desktop/mapData/%1/%2_%3_%4.png" ;
    QString imageMapPath = path.arg(zoom).arg(zoom).arg(grab.x()).arg(grab.y());
    QImage img(imageMapPath);
    //QPoint tp = grab;

    m_tilePixmaps[grab] = QPixmap::fromImage(img);
    if (img.isNull())
        m_tilePixmaps[grab] = m_emptyTile;
    emit updated(tileRect(grab));

    // purge unused spaces
    QRect bound = m_tilesRect.adjusted(-2, -2, 2, 2);
    foreach(QPoint grab, m_tilePixmaps.keys())
    if (!bound.contains(grab))
        m_tilePixmaps.remove(grab);

    LoadMap();

}

// ham nay thiet lap cac thu truoc khi thuc hien down,
//bang cach goi lenh hanleDatanetwork
void SlippyMap::download()
{
    QPoint grab(0, 0);
    for (int x = 0; x <= m_tilesRect.width(); ++x)
        for (int y = 0; y <= m_tilesRect.height(); ++y) {
            QPoint tp = m_tilesRect.topLeft() + QPoint(x, y);
            if (!m_tilePixmaps.contains(tp)) {
                grab = tp;
                break;
            }
        }
    if (grab == QPoint(0, 0)) {
        m_url = QUrl();
        return;
    }

    //khoi tao cac thong so de request du lieu, doan nay nen sua thanh load du lieu
    QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
    m_url = QUrl(path.arg(zoom).arg(grab.x()).arg(grab.y()));
    QNetworkRequest request;
    request.setUrl(m_url);
    request.setRawHeader("User-Agent", "Nokia (Qt) Graphics Dojo 1.0");
    // send chinh diem grab len lam gia tri de ham hanle nhan dc, cua user
    request.setAttribute(QNetworkRequest::User, QVariant(grab));
    m_manager.get(request);
}

// ham xac dinh vi tri manh ban do voi diem top-left cua no la diem tp
QRect SlippyMap::tileRect(const QPoint &tp)
{
    QPoint t = tp - m_tilesRect.topLeft();
    int x = t.x() * tdim + m_offset.x();
    int y = t.y() * tdim + m_offset.y();
    return QRect(x, y, tdim, tdim);
}
