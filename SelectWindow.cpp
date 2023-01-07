#include "SelectWindow.h"
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QLabel>
#include <QFile>

SelectWindow::SelectWindow(QWidget *parent)
    : QMainWindow{parent}
{
    setFixedSize(900,600);
    setWindowIcon(QIcon(":/image/SmallLogo.png"));
    setWindowTitle("塔防游戏选择关卡界面");
    //返回按钮
    MyPushButton * backbtn = new MyPushButton(":/image/Almanac_IndexButton.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width()-10,this->height()-backbtn->height()-10);
    connect(backbtn,&MyPushButton::clicked,[=]{
        emit this->select_back();
    });
    for(int i = 0; ; i++)
    {
        QString filename = QString("%1.txt").arg(i+1);
        QFile file(filename);
        file.open(QIODeviceBase::ReadOnly);
        if(!file.isOpen())
        {
            file.close();
            break;
        }
        file.close();
        MyPushButton * selectnum = new MyPushButton(":/image/Button.png");
        selectnum->setParent(this);
        selectnum->move(100+ i%4 * 200,150+i/4*100);
        connect(selectnum,&MyPushButton::clicked,[=]{
            QString str = QString("选择的是第 %1 关").arg(i+1);
            gamewindow = new GameWindow(i+1);
            gamewindow->show();
            this->hide();
            connect(gamewindow,&GameWindow::select_back,[=]{
                this->show();
                delete gamewindow;
                gamewindow = NULL;
            });
        });
        QLabel * label = new QLabel;
        label->setParent(this);
        label->setFixedSize(selectnum->width(),selectnum->height());
        label->setText(QString::number(i+1));
        label->move(100+ i%4 * 200,150+i/4*100);
        label->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        label->setAttribute(Qt::WA_TransparentForMouseEvents);
    }
}

void SelectWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/image/Challenge_Background.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
}
