#ifndef ENEMY_H
#define ENEMY_H
#include "GameWindow.h"
#include "tower.h"
#include "bullet.h"
#include <QString>
#include <QPainter>
#include <QPoint>
#include <QSize>

class tower;
class GameWindow;
class bullet;
class TowerPos;

#define basicHp 300
#define basicSpeed 1
#define hpLength 60

class enemy: public QObject
{
    Q_OBJECT
protected:
    GameWindow * game;
    int waves;
    int hp;
    int current_hp;
    int attackPower;
    double speed;
    QSize size;
    QTimer * AttackTimer;
    int AttackTime;
    int currentAreaNum;
    QPoint pos;
    QPoint nextPos;
    QString picturePath;
    bool alive;
    bool stop;
    int type;
    bool slow;
public:

    tower * targetTower;
    QVector<tower *> towerList;
    //QVector<tower *> towerToAttackList;
    enemy(int _waves, GameWindow * _game);
    int getHp(){return hp;}
    int getAttackPower(){return attackPower;}
    int getSpeed(){return speed;}
    QPoint getPos(){return pos;}
    void move();
    void setHp(int _hp){this->hp = _hp;}
    void setPicture(QString s) { picturePath = s; }
    void draw(QPainter * painter);
    void setNotAlive(){alive = false;}
    void setNotStop(){stop = false;}
    bool getStop(){return stop;}
    void hurted(tower * attacker);
    void attack(tower * target);
    void findTowers();
    void setType(int _type);
    void dead();
public slots:
    void setAlive();
    void doAttack();
};

#endif // ENEMY_H
