#ifndef CMAPWIDGET_H
#define CMAPWIDGET_H

#include <QWidget>
#include <QtGui>
class CMap;
class QNetworkSession;
class downloadimage;

class CMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CMapWidget(QWidget *parent = 0);
    void setCenterMap(qreal mlat, qreal mlong);

signals:

public slots:
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);

private:
    CMap *myMap;

    QLabel* imageLabel;
    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;
};

#endif // CMAPWIDGET_H
