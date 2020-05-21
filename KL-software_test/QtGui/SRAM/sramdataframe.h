#ifndef SramDataFrame_H
#define SramDataFrame_H

#include <QByteArray>
#include <QDebug>

#define DISABLE "Disable"
#define STANDBY "Standby"
#define STATE_PARAM "-s"
#define STATE_VALUE_ON "on"
#define STATE_VALUE_OFF "off"

#define READ "Read"
#define ADDRESS_PARAM "-a"
#define NO_BYTES_PARAM "-n"

#define WRITE "Write"
#define ADDRESS_PARAM "-a"
#define DATA_PARAM "-d"

#define END_FRAME "\r\n"

class SramDataFrame
{
public:
    enum direction {Request = static_cast<char>(0x00), Response = 0x01};
    enum frameBytesPositions {SOFposition = 0, lengthPosition = 1, typePosition = 2, functionIDposition = 3};
    SramDataFrame(direction dir);
    SramDataFrame();

    void setFrameFunction(char functionID, QByteArray functionParameters = QByteArray());
    QByteArray prepareDataToSend();

    void addDataRead(QByteArray dataRead);
    bool waitingForMore();
    bool parseDataRead();
    QByteArray getFrameAsByteArray();    
    void resetSramDataFrame();

private:
    char startOfFrame;
    char length;
    char type;
    char APIcommandId;
    QByteArray APIparameters;
    int numberOfParameters;
    void clearAllFields();

    char checksum;

    QByteArray bytesAlreadyRead;
    int bytesRead;
    int bytesInReadFrame;
};


#endif //SramDataFrame_H
