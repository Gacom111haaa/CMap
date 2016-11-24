#ifndef DOWNLOADIMAGE_H
#define DOWNLOADIMAGE_H

#include <QObject>
#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class DownloadImage : public QObject
{
    Q_OBJECT
public:
    explicit DownloadImage(QUrl imageUrl,  QObject *parent = 0);
    virtual ~DownloadImage();
    QByteArray downloadData() const;

signals:
    void downloaded();

private slots:
    void fileDownload(QNetworkReply *pReply);

private:
    QNetworkAccessManager m_WebCtrl;
    QByteArray m_DownloadedData;

};

#endif // DOWNLOADIMAGE_H
