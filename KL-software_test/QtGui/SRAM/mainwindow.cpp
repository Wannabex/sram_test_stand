#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->guiInitialize();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete uartDevice;
}

void MainWindow::guiInitialize()
{
    this->uartDevice = new QSerialPort(this);

    ui->comboBoxStatus->insertItem(0, "Operational");
    ui->comboBoxStatus->insertItem(1, "Standby");
    ui->comboBoxStatus->insertItem(2, "Output disable");
    this->setUartPowerText("No power");
    this->setMemoryPowerText("No power");
    this->setLastOperationText("Memory not yet initialized", QColor(255,0,0));

    this->uartStatus = false;
    this->memoryPower = false;
    this->memoryStatus = QString("No power");
    this->savedAddress = new QByteArray();
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}

void MainWindow::setUartPowerText(QString text, QColor color)
{
    ui->textBrowserUart->setTextColor(color);
    ui->textBrowserUart->setText(text);
}

void MainWindow::setMemoryPowerText(QString text, QColor color)
{
    ui->textBrowserMemoryStatus->setTextColor(color);
    ui->textBrowserMemoryStatus->setText(text);
}

void MainWindow::setLastOperationText(QString text, QColor color)
{
    ui->textBrowserLast->setTextColor(color);
    ui->textBrowserLast->setText(text);
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

    this->serialReader = new MySerialReader(this->uartDevice);
    this->readingThread = new QThread;
    this->serialReader->moveToThread(this->readingThread);
    connect(readingThread, SIGNAL(started()), serialReader, SLOT(readFromPort()));
    connect(serialReader, SIGNAL(dataToLogs(QString)), this, SLOT(dataFromReader(QString)));

    this->addToLogs("Connected with serial port");

    this->uartStatus = true;
    this->setUartPowerText("Uart initialized", QColor(0,255,0));
    ui->groupBoxBasic->setEnabled(true);
    ui->groupBoxAdvanced->setEnabled(true);
    ui->pushButtonDisconnect->setEnabled(true);
}

void MainWindow::dataFromReader(QString serialData)
{
    QString message = QString("Received bytes: " + serialData);
    this->addToLogs(message);
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
    this->setUartPowerText("Uart disconnected");
    this->uartStatus = false;
    ui->pushButtonDisconnect->setEnabled(false);
    ui->groupBoxBasic->setEnabled(false);
    ui->groupBoxAdvanced->setEnabled(false);
}

void MainWindow::sendMessageToDevice(QByteArray messageBytes)
{
    if(this->uartDevice->isOpen() && this->uartDevice->isWritable())
    {
        qDebug() << messageBytes;
        this->uartDevice->write(messageBytes);
    }
    else
    {
        this->addToLogs("Can't send this message");
    }
}

void MainWindow::on_pushButtonPower_clicked()
{
    qDebug() << "on_pushButtonPower_clicked()";

    if(!this->memoryPower)
    {
        // send power on
        this->memoryPower = true;
        this->setMemoryPowerText("Memory power is on", QColor(0,255,0));
        this->setLastOperationText("Power turned on");
    }
    else
    {
        // send power off
        this->memoryPower = false;
        this->setMemoryPowerText("Memory power is off");
        this->setLastOperationText("Power turned off");
    }
}

void MainWindow::on_pushButtonWriteFull_clicked()
{
    qDebug() << "on_pushButtonWriteFull_clicked()";
    unsigned char value = 0x05;
    this->setLastOperationText("Full memory write commanded");
}

void MainWindow::on_pushButtonReadFull_clicked()
{  
    unsigned char addressByteL = 0xff;
    unsigned char addressByteM = 0xff;
    unsigned char addressByteH = 0x01;
    this->savedAddress->append(addressByteL);
    this->savedAddress->append(addressByteM);
    this->savedAddress->append(addressByteH);
    this->setLastOperationText("Full memory read commanded");
}



void MainWindow::on_pushButtonStatus_clicked()
{
   this->memoryStatus = ui->comboBoxStatus->currentText();
   qDebug() << this->memoryStatus;
   this->setLastOperationText("Memory status changed");
}

void MainWindow::on_pushButtonAddress_clicked()
{
    int addressValue = ui->spinBoxAddress->value();
    unsigned char addressByteL = addressValue & 0xff;
    unsigned char addressByteM = (addressValue >> 8) & 0xff;
    unsigned char addressByteH = (addressValue >> 16) & 0xff;

    qDebug() << QString::number(addressByteH) << QString::number(addressByteM) << QString::number(addressByteL);
    this->savedAddress->append(addressByteL);
    this->savedAddress->append(addressByteM);
    this->savedAddress->append(addressByteH);
    this->setLastOperationText("Address to read/write set");
}

void MainWindow::on_pushButtonWrite_clicked()
{    
    unsigned char writeValue = ui->spinBoxWriteValue->value();
    qDebug() << QString::number(writeValue);
    this->setLastOperationText("Memory write commanded");
}

void MainWindow::on_pushButtonRead_clicked()
{
    unsigned char numberToRead = ui->spinBoxNumberToRead->value();
    qDebug() << QString::number(numberToRead);
    this->setLastOperationText("Memory read commanded");
}

