#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPaintEvent>
#include <QWidget>

#include "videoplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent *event);

private:
    Ui::MainWindow *ui;

    VideoPlayer *mPlayer; //播放线程

    QImage mImage; //记录当前的图像
    QImage R_mImage; //2017.8.11---lizhen

    QString url; 

    bool open_red=false;

private slots:
    void slotGetOneFrame(QImage img);
    void slotGetRFrame(QImage img);///2017.8.11---lizhen
    bool slotOpenRed();        ///2017.8.12---lizhen
    bool slotCloseRed();       ///2017.8.12

};

#endif // MAINWINDOW_H
