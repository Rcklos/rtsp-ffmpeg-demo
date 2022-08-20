#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QPainter>
#include <QInputDialog>
#include <QtMath>

#include<iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // 实例化播放器
    mPlayer = new VideoPlayer;
    // 绑定信号处理函数
    connect(mPlayer,SIGNAL(sig_GetOneFrame(QImage)),this,SLOT(slotGetOneFrame(QImage)));
    // 和上面一样，不过是小窗的信号
    connect(mPlayer,SIGNAL(sig_GetRFrame(QImage)),this,SLOT(slotGetRFrame(QImage)));
    // 打开关闭小窗(ui这里主要是显示红色分量)
    connect(ui->Open_red,&QAction::triggered,this,&MainWindow::slotOpenRed);
    connect(ui->Close_Red,&QAction::triggered,this,&MainWindow::slotCloseRed);

    // 启动播放器
    mPlayer->startPlay();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *event) {

    QPainter painter(this);

    // 背景
    painter.setBrush(Qt::white);
    painter.drawRect(0, 0, this->width(), this->height()); 

    if (mImage.size().width() <= 0) return;

    ///将图像按比例缩放成和窗口一样大小
    QImage img = mImage.scaled(this->size(),Qt::KeepAspectRatio);

    int x = this->width() - img.width();
    int y = this->height() - img.height();

    x /= 2;
    y /= 2;

    // 居中
    painter.drawImage(QPoint(x,y),img); 

    // 小窗
    if(open_red==true) {
        QWidget *red_video=new QWidget(this);
        red_video->resize(this->width()/3,this->height()/3);
        //提取出图像中的R数据
        painter.setBrush(Qt::white);
        painter.drawRect(0, 0, red_video->width(),red_video->height());

        if (R_mImage.size().width() <= 0) return;

        ///将图像按比例缩放成和窗口一样大小
        QImage R_img = R_mImage.scaled(red_video->size(),Qt::KeepAspectRatio);

        int R_x = red_video->width() - R_img.width();
        int R_y = red_video->height() - R_img.height();

        R_x /= 2;
        R_y /= 2;

        painter.drawImage(QPoint(R_x,R_y),R_img);  //画出图像
    }
}

void MainWindow::slotGetOneFrame(QImage img) {
    mImage = img;
    update(); //调用update将执行 paintEvent函数
}

// 小窗显示
void MainWindow::slotGetRFrame(QImage img) {
    R_mImage = img;
    update(); //调用update将执行 paintEvent函数
}
bool MainWindow::slotOpenRed() {
    open_red=true;
    return open_red;
}
bool MainWindow::slotCloseRed() {
    open_red=false;
    return open_red;
}





