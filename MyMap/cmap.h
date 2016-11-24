#ifndef CMAP_H
#define CMAP_H

#include <QtNetwork/QNetworkAccessManager>
#include <QPixmap>
#include <QUrl>

class QNetworkReply;
class QPainter;

class CMap: public QObject
{
    Q_OBJECT

public:
    CMap(QObject *parent = 0);
    virtual ~CMap();

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

private slots:
    //void handleNetworkData(QNetworkReply *reply);
    //void download();
    void LoadMap();

protected:
    QRect tileRect(const QPoint &tp);

private:
    QPoint m_offset;
    QRect m_tilesRect;
    QPixmap m_emptyTile;
    QHash<QPoint, QPixmap> m_tilePixmaps;
    //QNetworkAccessManager m_manager;
    QUrl m_url;
    QString mPath;

    int mMapWidth;
    int mMapHeight;
    double mScale;
    double mCenterLat;
    double mCenterLon;
};

#endif // CMAP_H
