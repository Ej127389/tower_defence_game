#include "tower.h"
#include <QPixmap>
#include <QLabel>
#include <QMovie>

const QSize tower::size(70,70);

void tower::draw(QPainter * painter)
{
    painter->save();
    QPoint drawPos = QPoint(pos.x() - size.width()/2, pos.y() - size.height()/2);
    QPixmap pix;
    pix.load(picturePath);
    painter->drawPixmap(drawPos.x(),drawPos.y(),size.width(),size.height(),pix);
    if(type == RemoteTower)
    {
        painter->setPen(Qt::blue);
        painter->drawEllipse(pos.x()-range,pos.y()-range,2*range,2*range);
    }
    if(type == MeleeTower)
    {
        QPoint hpStartPos = pos + QPoint(-hpLength/2,size.height()/2);
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::red);
        QRect healthBarBackRect(hpStartPos,QSize((double)currentHp / hp * hpLength,5));
        painter->drawRect(healthBarBackRect);
        painter->setBrush(Qt::red);
        QRect healthBarRect(hpStartPos, QSize((double)currentHp / hp * hpLength, 5));
        painter->drawRect(healthBarRect);
    }
    QPoint buffPos[] =
    {
        QPoint(pos.x() + size.width()/2, pos.y() - size.height()/2),
        QPoint(pos.x() + size.width()/2, pos.y() - size.height()/4),
        QPoint(pos.x() + size.width()/2, pos.y() + size.height()/4),
        QPoint(pos.x() + size.width()/2, pos.y() + size.height()/2)
    };
    painter->setPen(QPen(Qt::blue,4));
    painter->setBrush(QColor(255,0,0));
    if(buff_WateringCan)
        painter->drawEllipse(buffPos[0].x(),buffPos[0].y(),5,5);
    if(buff_Fertilizer)
        painter->drawEllipse(buffPos[1].x(),buffPos[1].y(),5,5);
    if(buff_bug_spray)
        painter->drawEllipse(buffPos[2].x(),buffPos[2].y(),5,5);
    if(buff_WateringCanGold)
        painter->drawEllipse(buffPos[3].x(),buffPos[3].y(),5,5);
    painter->restore();
}

bool tower::containPos(QPoint _pos)
{
    bool contain_x = pos.x() - size.width()/2 < _pos.x() && _pos.x() <  pos.x() + size.width()/2;
    bool contain_y = pos.y() - size.height()/2 < _pos.y() && _pos.y() <  pos.y() + size.height()/2;
    return contain_x && contain_y;
}

void tower::setType(int _type)
{
    type = _type;
    if(type == RemoteTower)
    {
        range = Range;
    }
    else if(type == MeleeTower)
    {
        range = 120;
    }
}

void tower::setBuff(bool b1, bool b2, bool b3, bool b4)
{
    buff_WateringCan = b1; buff_Fertilizer = b2;
    buff_bug_spray = b3; buff_WateringCanGold = b4;
    if(buff_WateringCan)
    {
        currentHp += 100;
    }
    else
    {
        currentHp -= 100;
        hp = 500;
    }
    if(buff_Fertilizer)
    {
        shootBulletTime -= 300;
    }
    else
    {
        shootBulletTime = 1000;
    }
    if(buff_bug_spray)
    {
        attackPower += 50;
    }
    else
    {
        attackPower = Power;
    }
}

void tower::shootBullet()
{
    if(targetEnemy != NULL)
    {
//        if(type == MeleeTower)
//            return;
        bullet * b = new bullet(game,pos,targetEnemy->getPos(),0,targetEnemy);
        b->setPicture(":/image/PB00.gif");
        game->addBullet(b);
        b->move();
        attack(targetEnemy);
    }
}

void tower::attack(enemy * target)
{
    if(target != NULL)
    {
        target->hurted(this);
        //target->dead();
    }
}

void tower::hurted(enemy * attacker)
{
    currentHp -= attacker->getAttackPower();
    if(currentHp <= 0)
    {
        //targetEnemy->towerList.removeOne(this);
        foreach(enemy * e, attackingEnemies)
        {
            e->targetTower = NULL;
            e->setNotStop();
            e->towerList.removeOne(this);

        }
        game->deleteTower(this);
        //attacker->findTowers();
    }

}

void tower::findEnemies()
{
    if(game->towerList.empty())
    {
        return;
    }
    if(targetEnemy != NULL)
    {
        if(!points_collision(pos,targetEnemy->getPos(),range,1))
        {
            //targetEnemy->towerList.removeOne(this);//
            targetEnemy = NULL;

        }
    }
    else if(targetEnemy == NULL)
    {
        foreach(enemy * e, game->enemyList)
        {
            if(points_collision(pos,e->getPos(),range,1))
            {
                targetEnemy = e;
                targetEnemy->towerList.push_back(this);
                shootBulletTimer->start(shootBulletTime);
                break;
            }
        }
    }

}


void tower::targetKilled()
{
    if(targetEnemy != NULL)
    {
        targetEnemy = NULL;
    }
    shootBulletTimer->stop();
}







