#ifndef SERIALPORTREADER_H
#define SERIALPORTREADER_H

#include <QObject>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QThread>

class MySerialReader : public QObject
{
     Q_OBJECT
public:
    MySerialReader(QSerialPort *device);
    void stopReader();

    QByteArray *sramFrameRead;
public slots:
    void readFromPort();

signals:
    void dataToLogs(QString serialData);    

private:
    bool running;    
    QSerialPort *readPort;
    QByteArray *receivedFrame;

    void checkForFullFrame(QByteArray dataRead);
    QString frameToString(QByteArray receivedBytes);

};

#endif // SERIALPORTREADER_H
