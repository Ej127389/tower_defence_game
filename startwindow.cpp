#include "startwindow.h"
#include "ui_startwindow.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QTimer>

startwindow::startwindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::startwindow)
{
    ui->setupUi(this);
    setFixedSize(900,600);
    setWindowIcon(QIcon(":/image/SmallLogo.png"));
    setWindowTitle("塔防游戏开始界面");
    MyPushButton * startbtn = new MyPushButton(":/image/SelectorScreenStartAdventur.png");
    startbtn->setParent(this);
    startbtn->move(475,70);
    //开始按钮操作
    select = new SelectWindow;
    connect(select,&SelectWindow::select_back,this,[=]{
        select->hide();
        this->show();
    });
    connect(startbtn,&MyPushButton::clicked,[=]{
        //做一个特效
        startbtn->zoom1();
        startbtn->zoom2();
        //延时进入
        QTimer::singleShot(300,this,[=]{
            //进入选择
            this->hide();
            select->show();
        });
    });
}

void startwindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/Surface.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    pix.load(":/image/SelectorScreen_WoodSign1_32.png");
    painter.drawPixmap(50,0,pix);
}

startwindow::~startwindow()
{
    delete ui;
}

