#include "mypushbutton.h"
#include <QDebug>
#include <QPropertyAnimation>

MyPushButton::MyPushButton(QString normalImg,QString pressImg)
{
    this->normalImgPath = normalImg;
    this->pressImgPath = pressImg;
    QPixmap pix;
    bool  ret = pix.load(normalImg);
    if(!ret)
    {
        qDebug() << "图片加载失败！";
    }
    this->setFixedSize(pix.width(),pix.height());
    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px;}");
    //设置图标
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

}

void MyPushButton::zoom1()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
void MyPushButton::zoom2()
{
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    animation->setDuration(200);
    //起始位置
    animation->setStartValue(QRect(this->x(),this->y()+5,this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}
