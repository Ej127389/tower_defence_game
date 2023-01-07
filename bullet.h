#ifndef BULLET_H
#define BULLET_H
#include <QString>
#include <QPainter>
#include <QPoint>
#include <QSize>
#include "GameWindow.h"
#include "tower.h"
#include "enemy.h"
#include <QPropertyAnimation>

class tower;
class GameWindow;
class enemy;

class bullet: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPoint currentPos READ getCurrentPos WRITE setCurrentPos)
protected:
    int attackPower;//攻击力
    QPoint startPos;
    QPoint endPos;
    QPoint currentPos;
    static const QSize size;
    int type;
    GameWindow * game;
    enemy * targetEnemy;
    QString picturePath;
public:
    bullet(GameWindow * _game, QPoint start, QPoint end, int _type, enemy * target):
        startPos(start),endPos(end),currentPos(start),type(_type),game(_game),targetEnemy(target){}
    int getPower(){return attackPower;}
    void setPicture(QString s) { picturePath = s; }
    void draw(QPainter * painter);
    void move();
    QPoint getCurrentPos(){return currentPos;}
    void setCurrentPos(QPoint _pos);
public slots:
    void hitTarget();
};

#endif // BULLET_H
