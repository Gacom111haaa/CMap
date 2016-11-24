#ifndef LIGHTMAP_H
#define LIGHTMAP_H

#include <QBasicTimer>
#include <QWidget>

class SlippyMap;

class Lightmap: public QWidget
{
    Q_OBJECT

public:
    Lightmap(QWidget *parent = 0);
    void setCenter(qreal lat, qreal lng);

protected:
    void paintEvent(QPaintEvent *event);

private:
    SlippyMap *m_myMap;

private slots:
    void updateMap(const QRect &r);

};

#endif // LIGHTMAP_H
