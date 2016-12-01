#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>


//class Lightmap;
//class SlippyMap;
class CMap;
class QNetworkSession;
class CMapWidget;

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
    void on_actionVien_CNTT_triggered();

    void on_verticalSlider_sliderReleased();

private:
    Ui::MainWindow *ui;
    //QNetworkSession *networkSession;
    CMapWidget *mapWidget;
    QVBoxLayout *vlayout;

};

#endif // MAINWINDOW_H
