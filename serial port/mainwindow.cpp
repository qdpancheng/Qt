#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Qt5 serial communication");
    //初始化
    initSerialPort();
}

void MainWindow::initSerialPort()
{
    connect(&serial, SIGNAL(readyRead()), this, SLOT(serialRead()));
    //get name for choose
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if (infos.isEmpty())
    {
        ui->comboBox->addItem("invalid");
        return;
    }
    foreach (QSerialPortInfo info, infos)
    {
        ui->comboBox->addItem(info.portName());
    }
}

MainWindow::~MainWindow()
{
    serial.close();
    delete ui;
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QSerialPortInfo info;
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    int i = 0;
    foreach (info, infos)
    {
        if (info.portName() == arg1)
            break;
        i++;
    }
    if (i != infos.size())
    {
        serial.close();
        serial.setPortName(arg1);
    }
    else
    {
        serial.close();
    }
}

void MainWindow::serialRead()
{
    ui->textEdit->append(serial.readAll());
}

void MainWindow::on_pushButton_clicked()
{
    if (ui->textEdit_2->toPlainText().isEmpty())
        return;
    serial.write(ui->textEdit_2->toPlainText().toLatin1());
    //ui->textEdit_2->toPlainText().
}

void MainWindow::on_comboBox_2_currentIndexChanged(const QString &arg1)
{
    if ("1200" == arg1)
        serial.setBaudRate(QSerialPort::Baud1200);
    else if ("2400" == arg1)
        serial.setBaudRate(QSerialPort::Baud2400);
    else if ("4800" == arg1)
        serial.setBaudRate(QSerialPort::Baud4800);
    else if ("9600" == arg1)
        serial.setBaudRate(QSerialPort::Baud9600);
    else if ("19200" == arg1)
        serial.setBaudRate(QSerialPort::Baud19200);
    else if ("38400" == arg1)
        serial.setBaudRate(QSerialPort::Baud38400);
    else if ("57600" == arg1)
        serial.setBaudRate(QSerialPort::Baud57600);
    else if ("115200" == arg1)
        serial.setBaudRate(QSerialPort::Baud115200);

}

void MainWindow::on_comboBox_3_currentIndexChanged(const QString &arg1)
{
    if ("NONE" == arg1)
        serial.setParity(QSerialPort::NoParity);
    else if ("ODD" == arg1)
        serial.setParity(QSerialPort::OddParity);
    else
        serial.setParity(QSerialPort::EvenParity);
}

void MainWindow::on_comboBox_4_currentIndexChanged(const QString &arg1)
{
    if ("8" == arg1)
        serial.setDataBits(QSerialPort::Data8);
    else if ("7" == arg1)
        serial.setDataBits(QSerialPort::Data7);
    else if ("6" == arg1)
        serial.setDataBits(QSerialPort::Data6);
}

void MainWindow::on_comboBox_5_currentIndexChanged(const QString &arg1)
{
    if ("2" == arg1)
        serial.setStopBits(QSerialPort::TwoStop);
    else if ("1" == arg1)
        serial.setStopBits(QSerialPort::OneStop);
}


void MainWindow::on_pushButton_2_clicked()
{
    if (ui->pushButton_2->text() == "OPEN")
    {
        ui->pushButton_2->setText("CLOSE");
        serial.open(QIODevice::ReadWrite);
    }
    else
    {
        ui->pushButton_2->setText("OPEN");
        serial.close();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->comboBox->clear();
    //get name for choose
    QList<QSerialPortInfo> infos = QSerialPortInfo::availablePorts();
    if (infos.isEmpty())
    {
        ui->comboBox->addItem("invalid");
        return;
    }
    foreach (QSerialPortInfo info, infos)
    {
        ui->comboBox->addItem(info.portName());
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    ui->textEdit_2->clear();
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->textEdit->clear();
}
