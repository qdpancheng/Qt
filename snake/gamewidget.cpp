#include "gamewidget.h"

const int ROW = 15;
const int COL = 15;
const int UP = 0;
const int DOWN = 1;
const int LEFT = 2;
const int RIGHT = 3;

GameWidget::GameWidget(QWidget *parent)
    : QWidget(parent)
    , direction(0)
    , foodx(0)
    , foody(0)
    , foodcount(0)
    , score(0)
    , level(4)
{
    this->setAutoFillBackground(true);          //覆盖父窗口
    //设置主窗口大小
    this->resize(500, 400);
    //设置标题
    this->setWindowIcon(QIcon(":/img/img/icon.png"));
    this->setWindowTitle(tr("贪吃蛇"));
    //设置背景色（并且对图片进行缩放）
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/img/green.jpg").scaled(this->size())));
    this->setPalette(palette);

    //分数
    score_label = new QLabel("score:", this);
    ft.setPointSize(16);
    score_label->setFont(ft);
    score_label->setGeometry(330, 8, 80, 35);
    score_lineEdit = new QLineEdit(this);
    ft.setPointSize(16);
    score_lineEdit->setFont(ft);
    score_lineEdit->setAlignment(Qt::AlignRight);
    score_lineEdit->setGeometry(410, 8, 80, 35);

    //等级
    level_label = new QLabel("level:", this);
    ft.setPointSize(16);
    level_label->setFont(ft);
    level_label->setGeometry(330, 50, 80, 35);
    level_lineEdit = new QLineEdit(this);
    ft.setPointSize(16);
    level_lineEdit->setFont(ft);
    level_lineEdit->setAlignment(Qt::AlignRight);
    level_lineEdit->setGeometry(410, 50, 80, 35);

    //按钮
    upbtn = new QPushButton("↑", this);
    upbtn->setGeometry(403, 115, 40, 30);
    leftbtn = new QPushButton("←", this);
    leftbtn->setGeometry(358, 155, 40, 30);
    downbtn = new QPushButton("↓", this);
    downbtn->setGeometry(403, 195, 40, 30);
    rightbtn = new QPushButton("→", this);
    rightbtn->setGeometry(448, 155, 40, 30);

    startbtn = new QPushButton("开始", this);
    startbtn->setGeometry(345, 245, 50, 30);
    stopbtn = new QPushButton("停止", this);
    stopbtn->setGeometry(400, 245, 50, 30);
    returnbtn = new QPushButton("返回", this);
    returnbtn->setGeometry(455, 245, 50, 30);

    connect(leftbtn, SIGNAL(clicked(bool)), this, SLOT(leftbtn_click()));
    connect(rightbtn, SIGNAL(clicked(bool)), this, SLOT(rightbtn_click()));
    connect(upbtn, SIGNAL(clicked(bool)), this, SLOT(upbtn_click()));
    connect(downbtn, SIGNAL(clicked(bool)), this, SLOT(downbtn_click()));
    connect(returnbtn, SIGNAL(clicked(bool)), this, SLOT(returnbtn_click()));
    connect(startbtn, SIGNAL(clicked(bool)), this, SLOT(startbtn_click()));
    connect(stopbtn, SIGNAL(clicked(bool)), this, SLOT(stopbtn_click()));
    connect(level_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(change_level()));

    //产生行列存储坐标
    snake[0][0] = qrand() % COL;        //行存储
    snake[0][1] = qrand() % ROW;        //列存储
    direction = qrand() % 4;            //方向控制
    //产生食物-----------------?
    int i = 0;
    do
    {
        i++;
        foodx = qrand() % COL;
        foody = qrand() % ROW;
    }while(foodx == snake[i][0] && foody == snake[i][1]);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timeroutslot()));

}

GameWidget::~GameWidget()
{
    delete score_label;
    delete score_lineEdit;
    delete upbtn;
    delete leftbtn;
    delete downbtn;
    delete rightbtn;
    delete startbtn;
    delete stopbtn;
    delete returnbtn;
    //delete timer;
}

void GameWidget::paintEvent(QPaintEvent *)
{
    int i, j;
    QPainter painter(this);
    //绘制游戏区
    for (i = 0; i < ROW; i++)
    {
        for (j = 0; j < COL; j++)
        {
            painter.drawRect(QRect(i * 20, j * 20, 20, 20));
        }
    }
    //
    painter.drawImage(QRectF(foodx * 20, foody * 20, 20, 20), QImage(":/img/img/apple.png"));

    //蛇头
    switch(direction)
    {
    case UP:
        painter.drawImage(QRectF(snake[0][0] * 20, snake[0][1] * 20, 20, 20), QImage(":/img/img/headup.png"));
        break;
    case DOWN:
        painter.drawImage(QRectF(snake[0][0] * 20, snake[0][1] * 20, 20, 20), QImage(":/img/img/headdown.png"));
        break;
    case LEFT:
        painter.drawImage(QRectF(snake[0][0] * 20, snake[0][1] * 20, 20, 20), QImage(":/img/img/headleft.png"));
        break;
    case RIGHT:
        painter.drawImage(QRectF(snake[0][0] * 20, snake[0][1] * 20, 20, 20), QImage(":/img/img/headright.png"));
        break;
    default:
        break;
    }

    //画蛇身
    for (i = 1; i < foodcount; i++)
    {
        //右下
        if ((snake[i][1] == snake[i - 1][1] && snake[i - 1][0] < snake[i][0] &&
             snake[i + 1][0] == snake[i][0] && snake[i + 1][1] < snake[i][1]) ||
             (snake[i -1][0] == snake[i][0] && snake[i - 1][1] < snake[i][1] &&
              snake[i][1] == snake[i + 1][1] && snake[i + 1][0] < snake[i][0]))
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/br_corner.png"));
        }
        //右上
        else if ((snake[i][0] == snake[i - 1][0] && snake[i][1] < snake[i - 1][1] &&
             snake[i + 1][1] == snake[i][1] && snake[i + 1][0] < snake[i][0]) ||
             (snake[i - 1][1] == snake[i][1] && snake[i - 1][0] < snake[i][0] &&
              snake[i][0] == snake[i + 1][0] && snake[i][1] < snake[i + 1][1]))
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/tr_corner.png"));
        }
        //左下
        else if ((snake[i][0] < snake[i - 1][0] && snake[i][1] == snake[i - 1][1] &&
           snake[i + 1][1] < snake[i][1] && snake[i][0] == snake[i + 1][0]) ||
           (snake[i - 1][1] < snake[i][1] && snake[i][0] == snake[i - 1][0] &&
            snake[i][0] < snake[i + 1][0] && snake[i][1] == snake[i + 1][1]))
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/bl_corner.png"));
        }
        //左上
        else if ((snake[i][0] == snake[i - 1][0] && snake[i][1] < snake[i - 1][1] &&
           snake[i][1] == snake[i + 1][1] && snake[i][0] < snake[i + 1][0]) ||
           (snake[i][1] == snake[i - 1][1] && snake[i][0] < snake[i - 1][0] &&
            snake[i][0] == snake[i + 1][0] && snake[i][1] < snake[i + 1][1]))
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/tl_corner.png"));
        }
        //水平方向
        else if (snake[i][1] == snake[i -1][1] && snake[i][0] != snake[i - 1][0])
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/h_body.png"));
        }
        //垂直方向
        else if (snake[i][1] != snake[i - 1][1] && snake[i][0] == snake[i - 1][0])
        {
            painter.drawImage(QRectF(snake[i][0] * 20, snake[i][1] * 20, 20, 20), QImage(":/img/img/v_body.png"));
        }
    }

    //蛇尾
    if ((snake[foodcount][1] == snake[foodcount - 1][1] && snake[foodcount][0] < snake[foodcount - 1][0]))
    {
        painter.drawImage(QRectF(snake[foodcount][0] * 20, snake[foodcount][1] * 20, 20, 20), QImage(":/img/img/tailright.png"));
    }
    else if ((snake[foodcount][1] == snake[foodcount - 1][1] && snake[foodcount - 1][0] < snake[foodcount][0]))
    {
        painter.drawImage(QRectF(snake[foodcount][0] * 20, snake[foodcount][1] * 20, 20, 20), QImage(":/img/img/tailleft.png"));
    }
    else if ((snake[foodcount][0] == snake[foodcount - 1][0] && snake[foodcount][1] < snake[foodcount - 1][1]))
    {
        painter.drawImage(QRectF(snake[foodcount][0] * 20, snake[foodcount][1] * 20, 20, 20), QImage(":/img/img/taildown.png"));
    }
    else if ((snake[foodcount][0] == snake[foodcount - 1][0] && snake[foodcount - 1][1] < snake[foodcount][1]))
    {
        painter.drawImage(QRectF(snake[foodcount][0] * 20, snake[foodcount][1] * 20, 20, 20), QImage(":/img/img/tailup.png"));
    }
}
//计算成绩
void GameWidget::change_score(int count)
{
    /*
    foodcount = 10  一级
    foodcount = 15  二级
    foodcount = 20  三级
    foodcount = 25  四级
    */

    if (count >= 1 && count <= 10)
    {
        //level = 1;
        score = count * 10;
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
        score_lineEdit->setText(QString(tr("%1")).arg(score));
    }
    else if(count > 10 && count <=15)
    {
        //level = 2;
        score = (count - 10) * 15 + 100;
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
        score_lineEdit->setText(QString(tr("%1")).arg(score));
    }
    else if (count > 15 && count <= 20)
    {
        //level = 3;
        score = (count - 15) * 20 + 175;
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
        score_lineEdit->setText(QString(tr("%1")).arg(score));
    }
    else if (count > 20 && count <= 25)
    {
        //level = 4;
        score = (count - 20) * 25 + 275;
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
        score_lineEdit->setText(QString(tr("%1")).arg(score));
    }
    else if(QMessageBox::question(this, tr("提示"), tr("you are very good !"),
            QMessageBox::Yes) == QMessageBox::Yes)
    {
        this->close();
        delete this;
        return;
    }
}


void GameWidget::upbtn_click()
{
    direction = UP;
}

void GameWidget::downbtn_click()
{
    direction = DOWN;
}

void GameWidget::leftbtn_click()
{
    direction = LEFT;
}

void GameWidget::rightbtn_click()
{
    direction = RIGHT;
}

void GameWidget::startbtn_click()
{
    //可以设置级别（改变蛇的移动时间）
    if (level == 1)
    {
        //---------------------------?
        timer->start(800);
    }
    else if (level == 2)
    {
        timer->start(600);
    }
    else if (level == 3)
    {
        timer->start(400);
    }
    else if (level == 4)
    {
        timer->start(200);
    }
    //---------------------------------?
    //level_lineEdit->setText(QString(tr("%1")).arg(level));
}

void GameWidget::stopbtn_click()
{
    timer->stop();
}

void GameWidget::returnbtn_click()
{
    //删除GameWidget的this指针----------？
    //delete timer;
    delete this;
}
//时间到的槽函数
void GameWidget::timeroutslot()
{
    int i = 0;
    //是否吃到食物
    if (snake[0][0] == foodx && snake[0][1] == foody)
    {
        //更新食物坐标,逻辑有问题，还可以优化改进
        do
        {
            i++;
            foodx = qrand() % COL;
            foody = qrand() % ROW;
        }while(foodx == snake[i][0] && foody == snake[i][1]);
        //改变分数函数
        change_score(++foodcount);
    }
    //保存数据
    memcpy(snake1, snake, sizeof(snake));
    //改变蛇的每一节身子坐标
    for (i = foodcount; i > 0; i--)
    {
        snake[i][0] = snake[i - 1][0];
        snake[i][1] = snake[i - 1][1];
    }
    switch(direction)
    {
    case UP:
        snake[0][1]--;
        break;
    case DOWN:
        snake[0][1]++;
        break;
    case LEFT:
        snake[0][0]--;
        break;
    case RIGHT:
        snake[0][0]++;
        break;
    default:
        break;
    }
    //蛇边界检测
    if (snake[0][0] < 0 || snake[0][0] >= COL || snake[0][1] < 0 || snake[0][1] >= ROW)
    {
        //恢复数据
        //memcpy(snake, snake1, sizeof(snake));
        if (QMessageBox::question(this, tr("提示"), tr("GAME OVER"), QMessageBox::Yes) == QMessageBox::Yes)
        {
            this->close();
            delete this;
            return;
        }
    }
    //蛇自杀检测
    if (foodcount >= 3)
    {
        for (i = 3; i <= foodcount; i++)
        {
            if (snake[0][0] == snake[i][0] && snake[0][1] == snake[i][1])
            {
                //恢复数据
                //memcpy(snake, snake1, sizeof(snake));
                if (QMessageBox::question(this, tr("提示"), tr("GAME OVER"), QMessageBox::Yes) == QMessageBox::Yes)
                {
                    this->close();
                    delete this;
                    return;
                }
            }
        }
    }
    //调用paintevent函数，重新绘制界面
    this->update();
}

void GameWidget::change_level()
{
    bool ok;
    int level1 = QInputDialog::getInt(this, tr("标准int类型输入对话框"),
        tr("请输入等级1-4"), level_lineEdit->text().toInt(&ok), 1, 4, 1, &ok);
    if (ok)
    {
        level = level1;
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
    }
    else
    {
        //level_lineEdit->setText(QString(tr("%1")).arg(level));
    }
}


