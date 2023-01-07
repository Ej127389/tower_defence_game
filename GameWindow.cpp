#include "GameWindow.h"
#include "mypushbutton.h"
#include <QPainter>
#include <QFile>
#include <QString>
#include <QDebug>
#include <QMap>
#include "bullet.h"
#include "enemy.h"
#include "tower.h"
#include <QtGlobal>

#define Towercost 100
#define Buffcost 75


GameWindow::GameWindow(int levelNum):
    playerHp(PlayHp),waves(0),money(500),gameEnd(false),gameWin(false)
{
    setFixedSize(900,600);
    setWindowIcon(QIcon(":/image/SmallLogo.png"));
    setWindowTitle("塔防游戏游戏界面");
    //返回按钮
    MyPushButton * backbtn = new MyPushButton(":/image/Almanac_IndexButton.png");
    backbtn->setParent(this);
    backbtn->move(this->width()-backbtn->width(),this->height()-backbtn->height());
    connect(backbtn,&MyPushButton::clicked,[=]{
        emit this->select_back();
    });
    QString filename = QString("%1.txt").arg(levelNum);
    QFile file(filename);
    file.open(QIODeviceBase::ReadOnly);
    if(!file.isOpen())
    {
        qDebug()<<"关卡未被创建，不存在！！！";
        return;
    }
    QTextStream stream(&file);
    int x;
    QMap<int,QPoint> tempMap;
    for(int i=0;i<5;i++)
    {
        QVector<int> temp;
        for(int j=0;j<10;j++)
        {
            stream >> x;
            temp.push_back(x);
            if(x == 0)
            {
                TowerPos tempPos(QPoint(150+j*75+33,62+i*104+52));
                remotetowerPosList.push_back(tempPos);
            }
            else
            {

                tempMap.insert(x,QPoint(150+j*75+33,62+i*104+52));
            }
        }
        map.push_back(temp);
    }
    file.close();
    QMapIterator<int, QPoint> iter(tempMap);
    while (iter.hasNext()) {
        iter.next();
        TowerPos tempPos(iter.value());
        meleetowerPosList.push_back(tempPos);
        enemyPosList.push_back(iter.value());
    }

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateMap()));
    timer->start(30);
    addwave();
}

void GameWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if(gameEnd || gameWin)
    {
        QString text = gameEnd ? "you lose!":"you win";
        painter.setPen(QPen(Qt::red));
        painter.drawText(rect(),Qt::AlignCenter,text);
        return;
    }
    QPixmap pix;
    pix.load(":/image/background1unsodded.jpg");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制地图
    for(int i=0;i<map.size();i++)
    {
        for(int j=0;j<map[i].size();j++)
        {
            if(map[i][j] == 0)
            {
                pix.load(":/image/Almanac_Ground.jpg");
            }
            else//敌人的路径
            {
                pix.load(":/image/postsbg.jpg");
            }

            painter.drawPixmap(150+j*75,62+i*104,75,104,pix);
        }
    }
    pix.load(":/image/bgHeader.jpg");
    painter.drawPixmap(0,0,900,pix.height(),pix);
    pix.load(":/image/bgFooter.jpg");
    painter.drawPixmap(0,29,900,pix.height(),pix);
    pix.load(":/image/chanzi.png");
    painter.drawPixmap(900-pix.width()-5,5,pix.width(),pix.height(),pix);
    pix.load(":/image/GloomShroom.png");
    painter.drawPixmap(0,60,pix.width(),pix.height(),pix);
    pix.load(":/image/WallNut.png");
    painter.drawPixmap(0,120,pix.width(),pix.height(),pix);
    pix.load(":/image/WateringCan.png");
    painter.drawPixmap(0,180,60,60,pix);
    pix.load(":/image/Fertilizer.png");
    painter.drawPixmap(0,240,60,60,pix);
    pix.load(":/image/bug_spray.png");
    painter.drawPixmap(0,300,60,60,pix);

    QFont font;
    font.setFamily("楷体");
    font.setPointSize(18);
    painter.setFont(font);
    painter.drawText(QRect(400, 15, 200, 25), QString("WAVE : %1").arg(waves + 1));
    painter.drawText(QRect(30, 15, 200, 25), QString("HP : %1").arg(playerHp));
    painter.drawText(QRect(200, 15, 200, 25), QString("MONEY : %1").arg(money));
    font.setPointSize(12);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(QPen(Qt::red));
    painter.drawText(QRect(60, 180+30, 90, 60), QString("加血，近/远"));
    painter.drawText(QRect(60, 240+30, 90, 60), QString("攻速，近/远"));
    painter.drawText(QRect(60, 300+30, 90, 60), QString("伤害，近/远"));

    foreach (tower * t, towerList) {
        t->draw(&painter);
    }
    foreach (bullet * b, bulletList){
        b->draw(&painter);
    }
    foreach (enemy * e, enemyList) {
        e->draw(&painter);
    }
}

void GameWindow::playHpDamage()
{
    playerHp--;
    if(playerHp <= 0)
    {
        gameEnd = true;
    }
}

void GameWindow::addTower(tower * t)
{
    towerList.push_back(t);
}

void GameWindow::deleteTower(tower * t)
{
    auto it = remotetowerPosList.begin();
    while(it != remotetowerPosList.end())
    {
        if(it->getPos() == t->getPos())
        {
            it->setnothasTower();
            it->setTower(NULL);
            break;
        }
        it++;
    }
    it = meleetowerPosList.begin();
    while(it != meleetowerPosList.end())
    {
        if(it->getPos() == t->getPos())
        {
            it->setnothasTower();
            it->setTower(NULL);
            break;
        }
        it++;
    }

    towerList.removeOne(t);
    delete t;
    t = NULL;
}

void GameWindow::addEnemy(enemy * e)
{
    enemyList.push_back(e);
}

void GameWindow::deleteEnemy(enemy * e)
{
    enemyList.removeOne(e);
//    e->targetTower = NULL;

    foreach(tower * t, e->towerList)
    {
        //qDebug()<<(t->targetEnemy==e);
        //t->targetEnemy = NULL;
        t->targetKilled();
    }
    delete e;
    e = NULL;
    money += 50;
    if(enemyList.empty())
    {
        waves++;
        bool res = addwave();
        if(!res)
        {
            gameWin = true;
        }
    }

}

void GameWindow::addBullet(bullet * b)
{
    bulletList.push_back(b);
}

void GameWindow::deleteBullet(bullet * b)
{
    bulletList.removeOne(b);
    delete b;
    b = NULL;
}

void GameWindow::mousePressEvent(QMouseEvent *event)//重载鼠标单击事件
{
    QPoint current_Pos = event->pos();
    static int state = 0;
    // 如果是鼠标左键按下
    if(event->button() == Qt::LeftButton)
    {
        if(point_in_area(current_Pos,QSize(100,60),QPoint(0,60)))
        {
            state = RemoteTower;
            qDebug()<<"选中了远战塔！";
        }
        else if(point_in_area(current_Pos,QSize(100,60),QPoint(0,120)))
        {
            state = MeleeTower;
            qDebug()<<"选中了近战塔！";
        }
        else if(point_in_area(current_Pos,QSize(50,50),QPoint(845,5)))
        {
            state = chanzi;
            qDebug()<<"选中了铲子！";
        }
        else if(point_in_area(current_Pos,QSize(60,60),QPoint(0,180)))
        {
            state = WateringCan;
            qDebug()<<"选中了喷水壶！";
        }
        else if(point_in_area(current_Pos,QSize(60,60),QPoint(0,240)))
        {
            state = Fertilizer;
            qDebug()<<"选中了化肥袋！";
        }
        else if(point_in_area(current_Pos,QSize(60,60),QPoint(0,300)))
        {
            state = bug_spray;
            qDebug()<<"选中了杀虫剂！";
        }
        auto it = remotetowerPosList.begin();
        while(it != remotetowerPosList.end())
        {
            if(state == RemoteTower && money >= Towercost && !it->HasTower()
                && it->containPos(current_Pos))
            {
                qDebug()<<"种一个远战塔！";
                it->sethasTower();
                tower * t = new tower(this, 0, it->getPos());
                t->setType(RemoteTower);
                t->setPicture(":/image/GloomShroom.gif");
                addTower(t);
                it->setTower(t);
                money -= Towercost;
                update();
            }
            else if(state == chanzi && it->HasTower() && it->containPos(current_Pos))
            {
                qDebug()<<"铲掉一个塔！";
                it->setnothasTower();
                deleteTower(it->getTower());
                it->setTower(NULL);
                money += Towercost*0.8;
                update();
            }
            else if(state == WateringCan && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_WateringCan)
                {
                    it->getTower()->setNotBuff1();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff1();
                }
            }
            else if(state == Fertilizer && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_Fertilizer)
                {
                    it->getTower()->setNotBuff2();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff2();
                }
            }
            else if(state == bug_spray && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_bug_spray)
                {
                    it->getTower()->setNotBuff3();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff3();
                }
            }
            it++;
        }
        it = meleetowerPosList.begin();
        while(it != meleetowerPosList.end())
        {
            if(state == MeleeTower && money >= Towercost && !it->HasTower()
                && it->containPos(current_Pos))
            {
                qDebug()<<"种一个近战塔！";
                it->sethasTower();

                tower * t = new tower(this, 0, it->getPos());
                t->setType(MeleeTower);
                t->setPicture(":/image/WallNut.gif");
                addTower(t);
                it->setTower(t);
                money -= Towercost;
                update();
            }
            else if(state == chanzi && it->HasTower() && it->containPos(current_Pos))
            {
                qDebug()<<"铲掉一个塔！";
                it->setnothasTower();
                deleteTower(it->getTower());
                it->setTower(NULL);
                money += Towercost*0.8;
                update();
            }
            else if(state == WateringCan && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_WateringCan)
                {
                    it->getTower()->setNotBuff1();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff1();
                }
            }
            else if(state == Fertilizer && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_Fertilizer)
                {
                    it->getTower()->setNotBuff2();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff2();
                }
            }
            else if(state == bug_spray && it->HasTower() && it->containPos(current_Pos))
            {
                if(it->getTower()->buff_bug_spray)
                {
                    it->getTower()->setNotBuff3();
                }
                else if(money >= Buffcost)
                {
                    money -= Buffcost;
                    it->getTower()->setBuff3();
                }
            }

            it++;
        }
    }
    // 如果是鼠标右键按下
    else if(event->button() == Qt::RightButton)
    {

    }

}

bool GameWindow::addwave()
{
    if(waves >= WinWaves)
        return false;
    int enemyStartInterval[]={100,1100,2100,3100,4100};
    QPoint startPos = QPoint(900, enemyPosList[0].y());
    for(int i = 0; i < OneWaveNum; i++)
    {
        srand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int a = rand() % 101;
        enemy * e = new enemy(waves,this);

        if(a<=20)
        {
            e->setType(1);
        }
        else if(a>20&&a<=50)
        {
            e->setType(2);
        }
        e->setPicture(":/image/enemy.gif");
        addEnemy(e);
        QTimer::singleShot(enemyStartInterval[i], e, SLOT(setAlive()));
    }
    return true;
}

void GameWindow::updateMap()
{
    foreach(enemy * e, enemyList)
    {
        e->move();
        e->findTowers();
    }
    foreach(tower * t, towerList)
    {
        t->findEnemies();
    }

    update();
}






