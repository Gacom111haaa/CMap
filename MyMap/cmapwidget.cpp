#include "cmapwidget.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// tile size in pixels
const int tdim = 256;


uint qHash(const QPoint& p)
{
    return p.x() * 17 ^ p.y();
}

// xac dinh manh ban do tu dau ra dau vao
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

CMapWidget::CMapWidget(QWidget *parent) : QWidget(parent)
{
    m_emptyTile = QPixmap(tdim, tdim);
    m_emptyTile.fill(Qt::lightGray);

    //myMap = new CMap(this);
    setCenterPos(21.036264,105.774866);
    setScaleRatio(15);
    setWidthHeight(400,300);
    setPath("C:/Users/LamPT/Desktop/mapData");
    invalidate();
}

void CMapWidget::paintEvent(QPaintEvent *e)
{
    QPainter p;
    p.begin(this);
    render(&p, e->rect());

    p.setPen((Qt::black));
    p.drawText(rect(), Qt::AlignBottom|Qt::TextWordWrap,
               "Day la ham thu nghiem cua LamPT");
}

void CMapWidget::resizeEvent(QResizeEvent *)
{
    //myMap->width = width();
    //myMap->height = height();
    setWidthHeight(width(), height());
    invalidate();
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
            pan(delta);
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
         int zoom = (int) getScaleRatio();
         switch(zoom)
         {
         case 14: setScaleRatio(15);
             break;
         case 15: setScaleRatio(16);
             break;
         case 16: setScaleRatio(14);
             break;
         default: setScaleRatio(15);
             break;
         }

         invalidate();
         update();
     }
}

//phan cho CMap
double CMapWidget::getScaleRatio()
{
    return mScale;
}

void CMapWidget::Repaint()
{
    invalidate();
}

void CMapWidget::setPath(QString path)
{
    mPath = path + "/%1/%2_%3_%4.png";
}

void CMapWidget::setCenterPos(double lat, double lon)
{
    mCenterLat = lat;
    mCenterLon = lon;
    invalidate();
}

void CMapWidget::setScaleRatio(double scale)
{
    mScale = scale;
    //invalidate();
}

void CMapWidget::setWidthHeight(int width, int height)
{
    mMapWidth = width;
    mMapHeight = height;
    //invalidate();
}

//ham kien tao lai cac thong so khiu thay doi
void CMapWidget::invalidate()
{
    if (mMapWidth <= 0 || mMapHeight <= 0)
        return;

    // xac dinh diem ban do
    QPointF ct = tileForCoordinate(mCenterLat, mCenterLon, mScale);
    qreal tx = ct.x();
    qreal ty = ct.y();


    // top-left corner of the center tile
    //diem tren cung ben trai cua manh ban do trung tam
    int xp = mMapWidth / 2 - (tx - floor(tx)) * tdim;
    int yp = mMapHeight / 2 - (ty - floor(ty)) * tdim;

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
    int xe = static_cast<int>(tx) + (mMapWidth - xp - 1) / tdim;
    int ye = static_cast<int>(ty) + (mMapHeight - yp - 1) / tdim;

    // build a rect
    // xay dung ban do
    m_tilesRect = QRect(xs, ys, xe - xs + 1, ye - ys + 1);

    //neu duong link ko rong thi thuc hien dơnload
    if (m_url.isEmpty())
        LoadMap();//download();

    emit updated(QRect(0, 0, mMapWidth, mMapHeight));
}

//ham viet du lieu hien thi tren su kien Painter
void CMapWidget::render(QPainter *p, const QRect &rect)
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

void CMapWidget::pan(const QPoint &delta)
{
    QPointF dx = QPointF(delta) / qreal(tdim);
    QPointF center = tileForCoordinate(mCenterLat, mCenterLon, mScale) - dx;
    mCenterLat = latitudeFromTile(center.y(), mScale);
    mCenterLon = longitudeFromTile(center.x(), mScale);
    invalidate();
}

void CMapWidget::LoadMap()
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


    //QString path = "C:/Users/LamPT/Desktop/mapData/%1/%2_%3_%4.png" ;
    QString imageMapPath = mPath.arg(mScale).arg(mScale).arg(grab.x()).arg(grab.y());
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

// ham xac dinh vi tri manh ban do voi diem top-left cua no la diem tp
QRect CMapWidget::tileRect(const QPoint &tp)
{
    QPoint t = tp - m_tilesRect.topLeft();
    int x = t.x() * tdim + m_offset.x();
    int y = t.y() * tdim + m_offset.y();
    return QRect(x, y, tdim, tdim);
}
