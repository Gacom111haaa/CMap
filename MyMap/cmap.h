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



signals:


private slots:
    //void handleNetworkData(QNetworkReply *reply);
    //void download();


protected:


private:

};

#endif // CMAP_H
