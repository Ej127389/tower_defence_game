#include "bullet.h"
#include <QPixmap>
#include <QPropertyAnimation>

const QSize bullet::size(15,15);

void bullet::draw(QPainter * painter)
{
    QPixmap pix;
    pix.load(picturePath);
    painter->drawPixmap(currentPos.x(),currentPos.y(),size.width(),size.height(),pix);
}

void bullet::move()
{
    static const int duration = 100;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "currentPos");
    animation->setDuration(duration);
    animation->setStartValue(startPos);
    animation->setEndValue(endPos);
    connect(animation,SIGNAL(finished()),this,SLOT(hitTarget()));
    animation->start();
}

void bullet::hitTarget()
{
//    if(type == 0)//普通子弹
//    {

//    }
//    else if(type == 1)
//    {

//    }
//    else
//    {

//    }
//    if(game->enemyList.contains(targetEnemy))//目标受伤害
//    {
//        targetEnemy->hurted();
//    }
    game->deleteBullet(this);
}

void bullet::setCurrentPos(QPoint _pos)
{
    currentPos = _pos;
}







