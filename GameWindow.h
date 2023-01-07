#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QPoint>
#include "bullet.h"
#include "enemy.h"
#include "tower.h"
#include "otherclasses.h"
#include <QMouseEvent>
#include <QTime>

class tower;
class bullet;
class enemy;
class TowerPos;

#define WinWaves 5
#define OneWaveNum 5
#define PlayHp 5
#define RemoteTower 1
#define MeleeTower 10
#define chanzi 20
#define WateringCan 21
#define Fertilizer 22
#define bug_spray 23

static int cnt = 0;


class GameWindow : public QMainWindow
{
    Q_OBJECT
private:
    QVector<QVector<int>> map;
    QVector<TowerPos> remotetowerPosList;//不用顺序

    int playerHp;
    int waves;
    int money;
    bool gameEnd;
    bool gameWin;
public:
    QVector<bullet *> bulletList;
    QVector<tower *> towerList;
    QVector<enemy *> enemyList;
    QVector<QPoint> enemyPosList;//按起点到终点排列
    QVector<TowerPos> meleetowerPosList;
    GameWindow(int levelNum);
    void paintEvent(QPaintEvent *);
    void playHpDamage();
    void addTower(tower * t);
    void deleteTower(tower * t);
    void addEnemy(enemy * e);
    void deleteEnemy(enemy * e);
    void addBullet(bullet * b);
    void deleteBullet(bullet * b);
    void mousePressEvent(QMouseEvent *event);
    bool addwave();
public slots:
    void updateMap();
signals:
    void select_back();

};

#endif // GAMEWINDOW_H
