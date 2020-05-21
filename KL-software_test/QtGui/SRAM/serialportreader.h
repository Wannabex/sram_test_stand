#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>
#include "sramdataframe.h"

class MySerialReader : public QObject
{
     Q_OBJECT
public:
    MySerialReader(QSerialPort *device);
    void stopReader();
    SramDataFrame *SramFrameRead;

public slots:
    void readFromPort();

signals:
    void dataToLogs(QString serialData);
    void sendToZwave(char toSend);
    void correctFrameRead(QByteArray frameBytes);

private:
    bool running;    
    bool awaitingResponse;
    bool commandSentAgain;
    QSerialPort *readPort;

};

#endif // SERIALPORTREADER_H
