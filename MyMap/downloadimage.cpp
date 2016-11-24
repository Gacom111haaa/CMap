#include "downloadimage.h"

DownloadImage::DownloadImage(QUrl imageUrl,  QObject *parent) : QObject(parent)
{
    connect(&m_WebCtrl, SIGNAL (finished(QNetworkReply*)),SLOT (fileDownloaded(QNetworkReply*)));

    QNetworkRequest request(imageUrl);
    m_WebCtrl.get(request);
}

DownloadImage::~DownloadImage(){}

void DownloadImage::fileDownload(QNetworkReply *pReply)
{
    m_DownloadedData = pReply->readAll();

    //emit signal
    pReply->deleteLater();
    emit downloaded();
}

QByteArray DownloadImage::downloadData() const
{
    return m_DownloadedData;
}
