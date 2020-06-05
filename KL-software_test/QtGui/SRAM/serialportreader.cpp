#include "serialportreader.h"
#include "QDebug"
#include "mainwindow.h"

MySerialReader::MySerialReader(QSerialPort *uartDevice)
{
    this->readPort = uartDevice;
    this->running = true;
    this->receivedFrame = new QByteArray();
}

void MySerialReader::readFromPort()
{
    while(this->running)
    {
        QByteArray dataBuffer = this->readPort->readAll();
        if (dataBuffer.size() > 0)
        {
            emit dataToLogs(dataBuffer);
            //this->checkForFullFrame(dataBuffer);
        }
        while(this->readPort->waitForReadyRead(50))// && this->readPort->canReadLine())
        {
        }
    }
}

void MySerialReader::checkForFullFrame(QByteArray dataRead)
{
    // \r == 0x0D \n == 0x0A
    receivedFrame->append(dataRead);
    int dataLength = receivedFrame->length();
    if( (receivedFrame->at(dataLength - 2) == 0x0D) &&
        (receivedFrame->at(dataLength - 1) == 0x0A) )
    {
        QString logsMessage = frameToString(*receivedFrame);
        receivedFrame->clear();
        emit dataToLogs(logsMessage);
    }
}

QString MySerialReader::frameToString(QByteArray receivedBytes)
{
    QString bytesAsString;
    QByteArray::iterator byteCounter;
    for(byteCounter = receivedBytes.begin(); byteCounter != receivedBytes.end(); byteCounter++)
    {
        bytesAsString.append(*byteCounter);
    }
    return bytesAsString;
}


void MySerialReader::stopReader()
{
    this->running = false;
}

