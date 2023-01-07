#ifndef STARTWINDOW_H
#define STARTWINDOW_H
#include "SelectWindow.h"
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class startwindow; }
QT_END_NAMESPACE

class startwindow : public QMainWindow
{
    Q_OBJECT

public:
    startwindow(QWidget *parent = nullptr);
    ~startwindow();

    //重新paintEvent事件，画背景图
    void paintEvent(QPaintEvent *);
    SelectWindow * select = NULL;

private:
    Ui::startwindow *ui;
};
#endif // STARTWINDOW_H
