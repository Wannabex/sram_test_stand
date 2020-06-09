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
    this->receivedFrameNumber = 1;
    this->transmittedFrameNumber = 1;

    ui->comboBoxStatus->insertItem(0, "Operational");
    ui->comboBoxStatus->insertItem(1, "Standby");
    ui->comboBoxStatus->insertItem(2, "Output disable");
    this->setUartPowerText("No power");
    this->setMemoryPowerText("No power");
    this->setLastOperationText("Memory not yet initialized", QColor(255,0,0));

    this->uartStatus = false;
    this->memoryPower = false;
    this->memoryStatus = QString("No power");
}

void MainWindow::addToLogs(QString message)
{
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy.MM.dd.hh.mm.ss");
    ui->textEditLogs->append(currentDateTime + "\t" + message);
}

void MainWindow::addToFramesLog(QString frame, bool readNotwrite)
{
    if(readNotwrite)
    {
         ui->textEditFrames->append("Received frame number " + QString::number(receivedFrameNumber) + ":  " + frame);
         this->receivedFrameNumber++;
    }
    else
    {
        ui->textEditFrames->append("Transmitted frame number " + QString::number(transmittedFrameNumber) + ":  " + frame);
        this->transmittedFrameNumber++;
    }
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
    this->readingThread->start();
    this->addToLogs("Connected with serial port");

    this->uartStatus = true;
    this->setUartPowerText("Uart initialized", QColor(0,255,0));
    this->setMemoryPowerText("Memory operational", QColor(0,255,0));
    this->setLastOperationText("Test stand connected", QColor(0,0,0));

    ui->pushButtonReadFull->setEnabled(true);
    ui->pushButtonWriteFull->setEnabled(true);
    ui->pushButtonRead->setEnabled(true);
    ui->pushButtonWrite->setEnabled(true);
    ui->pushButtonDisconnect->setEnabled(true);
}

void MainWindow::dataFromReader(QString serialData)
{
    QString logMessage = QString("Received data frame number " + QString::number(this->receivedFrameNumber));
    this->addToLogs(logMessage);
    QString logsData = serialData;
    logsData.chop(2); // remove \r\n at the end
    this->addToFramesLog(logsData, 1);
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

    ui->pushButtonReadFull->setEnabled(false);
    ui->pushButtonWriteFull->setEnabled(false);
    ui->pushButtonRead->setEnabled(false);
    ui->pushButtonWrite->setEnabled(false);    
}

void MainWindow::sendMessageToDevice(QByteArray messageBytes)
{    
    if(this->uartDevice->isOpen() && this->uartDevice->isWritable())
    {
        QByteArray logsMessage = messageBytes;
        logsMessage.chop(2); // remove \r\n at the end
        this->addToFramesLog(logsMessage, 0);
        this->uartDevice->write(messageBytes);
    }
    else
    {
        this->addToLogs("Can't send this message");
    }
}

void MainWindow::on_pushButtonPower_clicked()
{
    if(!this->memoryPower)
    {
        // send power on
        //this->sendMessageToDevice();
        this->memoryPower = true;
        this->setMemoryPowerText("Memory power is on", QColor(0,255,0));
        this->setLastOperationText("Power switch on");
        this->addToLogs("Memory power switched on");
    }
    else
    {
        // send power off
        //this->sendMessageToDevice();
        this->memoryPower = false;
        this->setMemoryPowerText("Memory power is off");
        this->setLastOperationText("Power switch off");
        this->addToLogs("Memory power switched off");
    }
}

void MainWindow::on_pushButtonWriteFull_clicked()
{
    QByteArray messageFrame = QByteArray();
    QString asciiBytes;
    unsigned char byteValue = 0x55;
    this->setLastOperationText("Write full memory");
    this->addToLogs("Full memory write started");


    int numberOfWrites = MEMORY_WORDS / MAX_BYTES_TO_READWRITE;

    for (int writeOperaion = 0; writeOperaion < numberOfWrites; writeOperaion++)
    {
        messageFrame.append(WRITE);
        messageFrame.append(" ");
        int startingAddress = 0 + writeOperaion * MAX_BYTES_TO_READWRITE;
        asciiBytes.setNum(startingAddress, 16);
        messageFrame.append(this->formatAsciiBytes(asciiBytes, 5));
        messageFrame.append(" ");
        asciiBytes.setNum(MAX_BYTES_TO_READWRITE, 16);
        messageFrame.append(" ");
        for (int byteToWrite = 0; byteToWrite < MAX_BYTES_TO_READWRITE; byteToWrite++)
        {
            asciiBytes.setNum(byteValue, 16);
            messageFrame.append(this->formatAsciiBytes(asciiBytes));
            messageFrame.append(" ");            
        }
        messageFrame.append(END_FRAME);
        this->sendMessageToDevice(messageFrame);
        messageFrame.clear();
    }
}

void MainWindow::on_pushButtonReadFull_clicked()
{  
    QByteArray messageFrame = QByteArray();   
    QString asciiBytes;

    this->setLastOperationText("Read full memory");
    this->addToLogs("Full memory read started");
    int numberOfReads = MEMORY_WORDS / MAX_BYTES_TO_READWRITE;

    for (int readOperaion = 0; readOperaion < numberOfReads; readOperaion++)
    {
        messageFrame.append(READ);
        messageFrame.append(" ");
        int startingAddress = 0 + readOperaion * MAX_BYTES_TO_READWRITE;
        asciiBytes.setNum(startingAddress, 16);
        messageFrame.append(this->formatAsciiBytes(asciiBytes, 5));
        messageFrame.append(" ");
        asciiBytes.setNum(MAX_BYTES_TO_READWRITE, 16);
        messageFrame.append(" ");
        messageFrame.append(this->formatAsciiBytes(asciiBytes));
        messageFrame.append(" ");
        messageFrame.append(END_FRAME);
        this->sendMessageToDevice(messageFrame);
        messageFrame.clear();
    }

}

void MainWindow::on_pushButtonStatus_clicked()
{
   QByteArray messageFrame = QByteArray();
   this->memoryStatus = ui->comboBoxStatus->currentText();
   this->setLastOperationText("Memory status change");
   this->addToLogs("Memory status changed");

    messageFrame.append(STATE);

   if(this->memoryStatus == "Operational")
   {
       messageFrame.append(STATE_OPERATIONAL);
   }
   else if(this->memoryStatus == "Standby")
   {
       messageFrame.append(STATE_STANDBY);
   }
   else
   {
       messageFrame.append(STATE_OUTPUT_DISABLE);
   }
   //messageFrame.append(END_FRAME);
   this->sendMessageToDevice(messageFrame);
}

void MainWindow::on_pushButtonWrite_clicked()
{    
    QByteArray messageFrame = QByteArray();
    QString asciiBytes;
    int numberToWrite = ui->spinBoxNumberToReadWrite->value();
    int writeValue = ui->spinBoxWriteValue->value();
    this->setLastOperationText("Write to memory starting from address");
    this->addToLogs("Memory write operation started");
    messageFrame.append(WRITE);
    messageFrame.append(" ");
    messageFrame.append(this->formatAsciiBytes(this->getOperationAddress(), 5));
    messageFrame.append(" ");
    asciiBytes.setNum(numberToWrite, 16);
    messageFrame.append(this->formatAsciiBytes(asciiBytes));
    messageFrame.append(" ");
    for (int byteToWrite = 0; byteToWrite < numberToWrite; byteToWrite++)
    {
        asciiBytes.setNum(writeValue, 16);
        messageFrame.append(this->formatAsciiBytes(asciiBytes));
        messageFrame.append(" ");
    }
    messageFrame.append(END_FRAME);
    this->sendMessageToDevice(messageFrame);
}

void MainWindow::on_pushButtonRead_clicked()
{
    QByteArray messageFrame = QByteArray();
    QString asciiBytes;
    unsigned char numberToRead = ui->spinBoxNumberToReadWrite->value();
    this->setLastOperationText("Read memory bytes starting from address");
    this->addToLogs("Memory read operation started");

    messageFrame.append(READ);
    messageFrame.append(" ");
    messageFrame.append(this->formatAsciiBytes(this->getOperationAddress(), 5));
    messageFrame.append(" ");
    asciiBytes.setNum(numberToRead, 16);
    messageFrame.append(this->formatAsciiBytes(asciiBytes));
    messageFrame.append(" ");
    messageFrame.append(END_FRAME);
    this->sendMessageToDevice(messageFrame);
}

QString MainWindow::getOperationAddress()
{
    QString operationAddress;
    int addressValue = ui->spinBoxAddress->value();
 /* unsigned char addressByteL = addressValue & 0xff;
    unsigned char addressByteM = (addressValue >> 8) & 0xff;
    unsigned char addressByteH = (addressValue >> 16) & 0xff;
 */
    operationAddress.setNum(addressValue, 16);

    return operationAddress;
}

QString MainWindow::formatAsciiBytes(QString asciiBytes, int targetLength)
{    
    QString formattedBytes;    
    if (asciiBytes.length() % 2 == 1)
    {
        formattedBytes.append("0");
    }
    formattedBytes.append(asciiBytes);

    if(targetLength > 0)
    {
        int currentLength = formattedBytes.length();
        int lengthDifference = targetLength - currentLength;
        for (int missingChar = 0; missingChar < lengthDifference; missingChar++)
        {
            formattedBytes.push_front('0');
        }
    }
    formattedBytes.push_front("0x");
    return formattedBytes;
}
