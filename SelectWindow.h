#ifndef SELECTWINDOW_H
#define SELECTWINDOW_H

#include <QMainWindow>
#include "GameWindow.h"

class SelectWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit SelectWindow(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *);
    GameWindow * gamewindow = NULL;
signals:
    void select_back();
};

#endif // SELECTWINDOW_H
