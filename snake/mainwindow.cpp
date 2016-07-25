#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->resize(500, 400);
    this->setWindowIcon(QIcon(":/img/img/icon.png"));
    this->setWindowTitle(tr("贪吃蛇"));

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/img/back.jpg").scaled(this->size())));
    this->setPalette(palette);

    //开始图标设置
    startbtn = new QPushButton(this);
    startbtn->setIcon(QIcon(":/img/img/start.png"));
    startbtn->setIconSize(QSize(80,80));
    startbtn->setGeometry(QRect(250, 250, 72, 72));
    //消除按钮边框
    startbtn->setFlat(true);

    //quit图标设置
    endbtn = new QPushButton(this);
    endbtn->setIcon(QIcon(":/img/img/quit.png"));
    endbtn->setIconSize(QSize(80, 80));
    endbtn->setGeometry(QRect(350, 250, 72, 72));
    endbtn->setFlat(true);

    //槽函数
    connect(startbtn, SIGNAL(clicked(bool)), this, SLOT(startbtn_clicked()));
    connect(endbtn, SIGNAL(clicked(bool)), this, SLOT(endbtn_clicked()));
}

MainWindow::~MainWindow()
{
    delete startbtn;
    delete endbtn;
    delete ui;
}

void MainWindow::startbtn_clicked()
{
    //此处new出一个窗口，后面需要使用delete释放资源
    gamewidget = new GameWidget(this);
    gamewidget->show();
}

void MainWindow::endbtn_clicked()
{
    //
    if (QMessageBox::question(this, tr("提示"), tr("您是否要退出"),
            QMessageBox::No | QMessageBox::Yes) == QMessageBox::Yes)
    {
        delete this;  //主窗口使用new的方式产生没有问题，否则会有问题
        exit(0);
    }

}

