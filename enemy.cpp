#include "enemy.h"
#include <QPixmap>
#include "otherclasses.h"


enemy::enemy(int _waves, GameWindow * _game):
    game(_game),waves(_waves),attackPower(60),AttackTime(500),currentAreaNum(0),
    alive(false),stop(false),type(0),slow(false),targetTower(NULL),size(QSize(70,70))
{
    hp = basicHp + 50 * waves;
    current_hp = basicHp + 50 * waves;
    speed = basicSpeed + 0.1 * waves;
    pos = game->enemyPosList[currentAreaNum];
    nextPos = game->enemyPosList[currentAreaNum + 1];
    AttackTimer = new QTimer(this);
    connect(AttackTimer, SIGNAL(timeout()), this, SLOT(doAttack()));
}


void enemy::move()
{
    if(currentAreaNum < game->enemyPosList.size() - 1 &&
        game->meleetowerPosList[currentAreaNum + 1].HasTower())
    {
        stop = true;
    }
//    if(game->meleetowerPosList[currentAreaNum + 1].HasTower())
//    {
//        stop = true;
//    }
    else
    {
        stop = false;
    }
    if(!alive || stop)
        return;
    if(points_collision(pos,nextPos,1,1))
    {
        if(currentAreaNum >= game->enemyPosList.size() - 2)
        {
            game->playHpDamage();
            game->deleteEnemy(this);
            //this->dead();
            return;
        }
        else
        {
            currentAreaNum++;
            pos = game->enemyPosList[currentAreaNum];
            nextPos = game->enemyPosList[currentAreaNum + 1];
        }
    }
    QPoint offset = nextPos - pos;
    if(offset.x() == 0)
    {
        if(offset.y() > 0)
        {
            pos += QPoint(0,1) * 2 * speed;
        }
        else
        {
            pos += QPoint(0,-1) * 2 * speed;
        }
    }
    else if(offset.x() > 0)
    {
        pos += QPoint(1,0) * 2 * speed;
    }
    else
    {
        pos += QPoint(-1,0) * 2 * speed;
    }
}

void enemy::draw(QPainter * painter)
{
    if(!alive)
        return;
    painter->save();
    int Length = hpLength;
    QFont font;
    font.setFamily("楷体");
    font.setPointSize(10);
    font.setBold(true);
    painter->setFont(font);
    painter->setPen(QPen(Qt::red));
    if(type==1)
    {
        painter->drawText(QRect(pos.x()-size.width()/2, pos.y()-size.height()/2, 70, 20), QString("Hp++"));

    }
    else if(type == 2)
    {
        painter->drawText(QRect(pos.x()-size.width()/2, pos.y()-size.height()/2, 70, 20), QString("加速"));

    }
    QPoint hpStartPos = pos + QPoint(-hpLength/2,size.height()/2);
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::red);
    QRect healthBarBackRect(hpStartPos,QSize((double)current_hp / hp * Length,5));
    painter->drawRect(healthBarBackRect);
    painter->setBrush(Qt::green);
    if(slow)
        painter->setBrush(Qt::blue);
    QRect healthBarRect(hpStartPos, QSize((double)current_hp / hp * Length, 5));
    painter->drawRect(healthBarRect);
    QPixmap pix;
    pix.load(picturePath);
    painter->drawPixmap(pos.x()-size.width()/2,pos.y()-size.height()/2,size.width(),size.height(),pix);
    painter->restore();
}

void enemy::setAlive(){alive = true;}

void enemy::hurted(tower * attacker)
{
    if(attacker == NULL)
    {
        return;
    }
    current_hp -= attacker->getAttackPower();
    if(current_hp <= 0)
    {
        foreach(tower * t, towerList)
        {
            t->targetEnemy = NULL;
        }
        //game->enemyList.removeOne(this);
        //dead();
        game->deleteEnemy(this);
        //attacker->findEnemies();

    }
}

void enemy::attack(tower * target)
{
    if(target != NULL)
        target->hurted(this);
}

void enemy::findTowers()
{
    if(!game->meleetowerPosList[currentAreaNum + 1].HasTower())
    {
        stop = false;
    }
    if(stop == false)
    {
        //AttackTimer->stop();
        return;
    }
    if(targetTower == NULL)
    {
        //qDebug()<<"进行攻击";
        if(!game->meleetowerPosList[currentAreaNum + 1].HasTower())
            return;
        targetTower = game->meleetowerPosList[currentAreaNum + 1].getTower();
        //attack(targetTower);
        targetTower->attackingEnemies.push_back(this);
        AttackTimer->start(AttackTime);
    }

}

void enemy::doAttack()
{
    attack(targetTower);
}

void enemy::setType(int _type)
{
    type = _type;
    if(type == 1)//hp大
    {
        hp = hp*1.25;
    }
    else if(type == 2)//速度快血薄
    {
        hp = hp*0.75;
        speed += 0.5;
    }
}

void enemy::dead()
{
    game->deleteEnemy(this);
}


