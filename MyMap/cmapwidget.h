#ifndef CMAPWIDGET_H
#define CMAPWIDGET_H

#include <QWidget>
#include <QtGui>
//class CMap;
//class QNetworkSession;
//class downloadimage;

class QPainter;

class CMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMapWidget(QWidget *parent = 0);
    //void setCenterMap(qreal mlat, qreal mlong);

    void Repaint();
    void setCenterPos(double lat, double lon);
    void setScaleRatio(double scale);
    void setWidthHeight(int width, int height);
    void setPath(QString path);
    double getScaleRatio();


    void pan(const QPoint &delta);
    void invalidate();
    void render(QPainter *p, const QRect &rect);

signals:
    void updated(const QRect &rect);

public slots:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);

protected:
    QRect tileRect(const QPoint &tp);
    void LoadMap();

private:
    //CMap *myMap;

    QLabel* imageLabel;
    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;


    QPoint m_offset;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QPoint, QPixmap> m_tilePixmaps;
    QUrl m_url;
    QString mPath;

    int mMapWidth;
    int mMapHeight;
    double mScale;
    double mCenterLat;
    double mCenterLon;
};

#endif // CMAPWIDGET_H
