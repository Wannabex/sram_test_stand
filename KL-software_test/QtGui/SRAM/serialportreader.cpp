#include "serialportreader.h"
#include "QDebug"
#include "mainwindow.h"

MySerialReader::MySerialReader(QSerialPort *device)
{
    this->readPort = device;
    this->running = true;
    this->awaitingResponse = false;
    this->SramFrameRead = new SramDataFrame();
    this->commandSentAgain = false;
}

void MySerialReader::readFromPort()
{
    while(this->running)
    {
        QByteArray dataBuffer = this->readPort->readAll();
        if (dataBuffer.size() > 0)
        {
            //this->checkForZFrame(dataBuffer);
        }

        while(this->readPort->waitForReadyRead(50))// && this->readPort->canReadLine())
        {
            //QString readZwave = this->readPort->readAll();
            //QString toLogs = this->readPort->readLine();




            //QString toLogs = QString::fromStdString(dataBuffer.toStdString());
            //emit dataToLogs(toLogs);
            //qDebug() << readZwave;
            //if (readZwave != "")
            //{
             //   emit dataToLogs(readZwave);
            //}
        }
    }
}

void MySerialReader::stopReader()
{
    this->running = false;
}

