#ifndef TOWER_H
#define TOWER_H
#include "otherclasses.h"
#include "GameWindow.h"
#include "bullet.h"
#include <QString>
#include <QPainter>
#include <QPoint>
#include <QSize>
#include <QTimer>
#include <QVector>

class GameWindow;
class bullet;
class enemy;

#define Range 150
#define Power 50
#define BuffConsistTime 5000//buff可持续时间为5s

class tower: public QObject
{
    Q_OBJECT
protected:
    GameWindow * game;
    int type;
    int hp;
    int currentHp;
    int attackPower;
    int range;
    int speed;
    QPoint pos;
    static const QSize size;
    QString picturePath;
    QTimer * shootBulletTimer;
    int shootBulletTime;
    int level;
    //QVector<enemy *> enemyList;
    bool makeSlowDown;
public:
    tower(GameWindow * _game, int _type, QPoint _pos):
        game(_game),type(_type),hp(500),attackPower(Power),range(Range),speed(1000),
        pos(_pos),shootBulletTime(1000),level(0),buff_WateringCan(false),buff_Fertilizer(false),
        buff_bug_spray(0),buff_WateringCanGold(false),targetEnemy(NULL),makeSlowDown(false)
    {
        currentHp = hp;
        shootBulletTimer = new QTimer(this);
        connect(shootBulletTimer, SIGNAL(timeout()), this, SLOT(shootBullet()));
    }
    bool buff_WateringCan;
    bool buff_Fertilizer;
    bool buff_bug_spray;
    bool buff_WateringCanGold;
    void setPicture(QString s) { picturePath = s; }
    void draw(QPainter * painter);
    bool containPos(QPoint _pos);
    void setType(int _type);
    int getLevel(){return level;}
    QPoint getPos(){return pos;}
    int getType(){return type;}
    int getAttackPower(){return attackPower;}
    void attack(enemy * target);
    void hurted(enemy * attacker);
    void findEnemies();
    void setBuff(bool b1, bool b2, bool b3, bool b4);
    void setBuff1(){setBuff(true,buff_Fertilizer,buff_bug_spray,buff_WateringCanGold);}
    void setNotBuff1(){setBuff(false,buff_Fertilizer,buff_bug_spray,buff_WateringCanGold);}
    void setBuff2(){setBuff(buff_WateringCan,true,buff_bug_spray,buff_WateringCanGold);}
    void setNotBuff2(){setBuff(buff_WateringCan,false,buff_bug_spray,buff_WateringCanGold);}
    void setBuff3(){setBuff(buff_WateringCan,buff_Fertilizer,true,buff_WateringCanGold);}
    void setNotBuff3(){setBuff(buff_WateringCan,buff_Fertilizer,false,buff_WateringCanGold);}
    void setBuff4(){setBuff(buff_WateringCan,buff_Fertilizer,buff_bug_spray,true);}
    void setNotBuff4(){setBuff(buff_WateringCan,buff_Fertilizer,buff_bug_spray,false);}
    enemy * targetEnemy;
    QVector<enemy *> attackingEnemies;
    void targetKilled();
public slots:
    void shootBullet();
};

#endif // TOWER_H
