#include "otherclasses.h"
#include <QtMath>

bool TowerPos::containPos(QPoint _pos)
{
    bool contain_x = pos.x() - size.width()/2 < _pos.x() && _pos.x() <  pos.x() + size.width()/2;
    bool contain_y = pos.y() - size.height()/2 < _pos.y() && _pos.y() <  pos.y() + size.height()/2;
    return contain_x && contain_y;
}

bool points_collision(QPoint role1, QPoint role2, int range1, int range2)
{
    int distance = qPow(role1.x()-role2.x(), 2) + qPow(role1.y()-role2.y(), 2);
    if(distance <= qPow(range1+range2, 2))
        return true;
    return false;
}

bool point_in_area(QPoint _pos, QSize size, QPoint pos)
//判断点_pos是否在点pos为起点的区域内
{
    bool contain_x = pos.x() < _pos.x() && _pos.x() <  pos.x() + size.width();
    bool contain_y = pos.y() < _pos.y() && _pos.y() <  pos.y() + size.height();
    return contain_x && contain_y;
}
