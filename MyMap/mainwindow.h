#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>


//class Lightmap;
//class SlippyMap;
class CMap;
class QNetworkSession;
class downloadimage;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_action_Draw_map_triggered();
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *e);
    void sessionOpened();

    void on_actionVien_CNTT_triggered();
    void on_actionDownload_From_URL_triggered();

private:
    Ui::MainWindow *ui;
    //Lightmap *map;
    //SlippyMap *myMap;
    CMap *myMap;
    void setCenterMap(qreal mlat, qreal mlong);
    QNetworkSession *networkSession;
    QLabel* imageLabel;
    bool pressed;
    bool snapped;
    QPoint pressPos;
    QPoint dragPos;

};

#endif // MAINWINDOW_H
