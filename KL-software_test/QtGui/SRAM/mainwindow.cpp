#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->uartDevice = new QSerialPort(this);    
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uartDevice;
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}


void MainWindow::on_pushButtonSearch_clicked()
{
    ui->comboBoxDevices->clear();
    this->addToLogs("Searching for COM devices...");
    QList <QSerialPortInfo> foundDevices;
    foundDevices = QSerialPortInfo::availablePorts();
    for (int deviceNumber = 0; deviceNumber < foundDevices.count(); deviceNumber++ )
    {
        this->addToLogs("Found device: " + foundDevices.at(deviceNumber).portName() + foundDevices.at(deviceNumber).description());
        ui->comboBoxDevices->addItem(foundDevices.at(deviceNumber).portName() + "\t" + foundDevices.at(deviceNumber).description());
    }
    if (ui->comboBoxDevices->count() == 0)
    {
        this->addToLogs("Could not find any connected devices");
    }
    else
    {
        ui->pushButtonConnect->setEnabled(true);
        ui->comboBoxDevices->setEnabled(true);
    }


}

void MainWindow::on_pushButtonConnect_clicked()
{
    QString portName = ui->comboBoxDevices->currentText().split("\t").first();
    this->uartDevice->setPortName(portName);
    this->testStandInit();

    this->addToLogs("Connected with serial port");
    ui->pushButtonDisconnect->setEnabled(true);
}

void MainWindow::testStandInit()
{
    if(this->uartDevice->isOpen())
    {
        this->uartDevice->clear();
        this->uartDevice->close();
        this->addToLogs("Reconnecing with serial port");
    }
    if(uartDevice->open(QSerialPort::ReadWrite))
    {
        if (this->uartDevice->isOpen())
        {
            this->uartDevice->setBaudRate(QSerialPort::Baud115200);
            this->uartDevice->setDataBits(QSerialPort::Data8);
            this->uartDevice->setParity(QSerialPort::NoParity);
            this->uartDevice->setStopBits(QSerialPort::OneStop);
            this->uartDevice->setFlowControl(QSerialPort::NoFlowControl);
        }
        else
        {
            this->addToLogs("Could not connect with serial port");
        }
    }
}

void MainWindow::on_pushButtonDisconnect_clicked()
{
    if(this->uartDevice->isOpen())
    {
        this->uartDevice->close();
        /*this->readingThread->terminate();
        if(this->readingThread->isRunning())
        {
            this->serialReader->stopReader();
            this->readingThread->terminate();
            this->readingThread->yieldCurrentThread();
        }
        delete this->serialReader;
        delete this->readingThread;
*/
        this->addToLogs("Connection closed.");
    }
    ui->pushButtonDisconnect->setEnabled(false);
}


void MainWindow::on_pushButtonPower_clicked()
{
    qDebug() << "on_pushButtonPower_clicked()";
}

void MainWindow::on_pushButtonWriteFull_clicked()
{
    qDebug() << "on_pushButtonWriteFull_clicked()";
}

void MainWindow::on_pushButtonReadFull_clicked()
{
    qDebug() << "on_pushButtonReadFull_clicked()";
}



void MainWindow::on_pushButtonStatus_clicked()
{
   qDebug() << "on_pushButtonStatus_clicked()";
}

void MainWindow::on_pushButtonAddress_clicked()
{
    qDebug() << "on_pushButtonAddress_clicked()";
}

void MainWindow::on_pushButtonWrite_clicked()
{
    qDebug() << "on_pushButtonWrite_clicked()";
}

void MainWindow::on_pushButtonRead_clicked()
{
    qDebug() << "on_pushButtonRead_clicked()";
}




