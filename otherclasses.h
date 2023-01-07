#ifndef OTHERCLASSES_H
#define OTHERCLASSES_H

#include <QPoint>
#include <QString>
#include <QSize>
#include "tower.h"

class tower;

class TowerPos
{
    QPoint pos;
    bool hasTower;
    QSize size;
    tower * _tower;
public:
    TowerPos(QPoint _pos):pos(_pos),hasTower(false),size(QSize(75,104)),_tower(NULL){}
    void sethasTower(){hasTower = true;}
    void setnothasTower(){hasTower = false;}
    QPoint getPos(){return pos;}
    bool HasTower(){return hasTower;}
    bool containPos(QPoint _pos);
    void setTower(tower * t){_tower = t;}
    tower * getTower(){return _tower;}
};


bool points_collision(QPoint role1, QPoint role2, int range1, int range2);

bool point_in_area(QPoint _pos, QSize size, QPoint pos);
#endif // OTHERCLASSES_H
