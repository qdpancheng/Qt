#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QIcon>
#include <QPalette>         //调色板
#include <QBrush>           //笔刷
#include <QPushButton>
#include <QPixmap>
#include <QPainter>         //画笔
#include <QPaintEvent>
#include <QImage>
#include <QTimer>
#include <QMessageBox>
#include <QLineEdit>
#include <QLabel>
#include <QFont>
#include <QInputDialog>



class GameWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    ~GameWidget();
    //重载虚函数。repaint以及update时调用次函数
    //区别：repaint是立即调用paintEvent，而update是执行几次才调用一次
    void paintEvent(QPaintEvent *);

private:
    QLabel *score_label;
    QLabel *level_label;
    QLineEdit *score_lineEdit;
    QLineEdit *level_lineEdit;
    QPushButton *leftbtn;
    QPushButton *rightbtn;
    QPushButton *upbtn;
    QPushButton *downbtn;
    QPushButton *startbtn;
    QPushButton *returnbtn;
    QPushButton *stopbtn;
    //QPushButton *gameoverbtn;
    QFont ft;
    int direction;      //蛇移动方向
    int foodx;
    int foody;
    int foodcount;
    int score;
    int level;
    QTimer *timer;
    int snake[100][2];      //保存蛇的每个节点的坐标
    int snake1[100][2];     //备份坐标数组
    void change_score(int count);

public slots:
    void leftbtn_click();
    void rightbtn_click();
    void upbtn_click();
    void downbtn_click();
    void startbtn_click();
    void stopbtn_click();
    void returnbtn_click();
    void timeroutslot();
    void change_level();

};


#endif // GAMEWIDGET_H
