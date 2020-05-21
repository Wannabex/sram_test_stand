#include "sramdataframe.h"

SramDataFrame::SramDataFrame(direction dir)
{
    //this->startOfFrame= SOF;
    this->length = 2;
    this->type = static_cast<char>(dir);
    this->APIcommandId = 0;
    this->APIparameters = QByteArray();
    this->numberOfParameters = 0;
    this->checksum = 0;

    this->numberOfParameters = 0;
}

SramDataFrame::SramDataFrame()
{
    this->startOfFrame= 0;
    this->length = 0;
    this->type = 0;
    this->APIcommandId = 0;
    this->checksum = 0;    
    this->numberOfParameters = 0;

    this->bytesAlreadyRead = QByteArray();
    this->bytesRead = 0;
    this->bytesInReadFrame = 50;
}

void SramDataFrame::setFrameFunction(char functionId, QByteArray functionParameters)
{
    this->clearAllFields();

    this->APIcommandId = functionId;
    this->length++;

    if (functionParameters.size() > 0)
    {
        for (int parameter = 0; parameter < functionParameters.size(); parameter++)
        {
            APIparameters.append(functionParameters[parameter]);
            this->numberOfParameters++;
            this->length++;
        }
    }
}

QByteArray SramDataFrame::prepareDataToSend()
{
    unsigned char calculatingChecksum = 0xff;
    QByteArray bytesToSend = QByteArray();

    bytesToSend.append(this->startOfFrame);

    calculatingChecksum ^= this->length;
    bytesToSend.append(this->length);

    calculatingChecksum ^= static_cast<char>(this->type);
    bytesToSend.append(static_cast<char>(this->type));

    calculatingChecksum ^= this->APIcommandId;
    bytesToSend.append(this->APIcommandId);

    if (this->numberOfParameters > 0)
    {
        for (int i = 0; i < this->numberOfParameters; i++)
        {
            calculatingChecksum ^= this->APIparameters[i];
            bytesToSend.append(this->APIparameters[i]);
        }
    }
    this->checksum = calculatingChecksum;
    bytesToSend.append(this->checksum);
    return bytesToSend;
}

bool SramDataFrame::parseDataRead()
{
    char calculatedChecksum = static_cast<char>(0xff);
    this->startOfFrame = static_cast<int>(this->bytesAlreadyRead[0]);

    this->length = static_cast<int>(this->bytesAlreadyRead[1]);
    calculatedChecksum ^= this->length;
    qDebug() << static_cast<int>(this->startOfFrame) << " < SOF";
    qDebug() << static_cast<int>(this->length) << " < len";

    QByteArray frameFunctionData;
    int byteReceived;
    for (byteReceived = 2; byteReceived <= (this->length); byteReceived++)
    {
        frameFunctionData.append(this->bytesAlreadyRead[byteReceived]);
        calculatedChecksum ^= static_cast<int>(this->bytesAlreadyRead[byteReceived]);
        qDebug() << static_cast<int>(this->bytesAlreadyRead[byteReceived]) << " << data";
    }
    this->checksum = static_cast<int>(this->bytesAlreadyRead[byteReceived]);

    this->type = frameFunctionData[0];
    this->APIcommandId = frameFunctionData[1];
    frameFunctionData.remove(0, 2);
    this->APIparameters = frameFunctionData;
    this->numberOfParameters = this->APIparameters.length();

    qDebug() << static_cast<int>(this->checksum) << " czeksumki " << static_cast<int>(calculatedChecksum);
    if (this->checksum == calculatedChecksum)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SramDataFrame::addDataRead(QByteArray dataRead)
{
    this->bytesAlreadyRead.append(dataRead);
    if (static_cast<char>(this->bytesAlreadyRead[lengthPosition]) > 0)
    {
        this->bytesInReadFrame = static_cast<char>(this->bytesAlreadyRead[lengthPosition]);
        this->bytesRead = this->bytesAlreadyRead.size();
    }
}

bool SramDataFrame::waitingForMore()
{
    if (this->bytesRead >= this->bytesInReadFrame)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QByteArray SramDataFrame::getFrameAsByteArray()
{
    QByteArray frameBytes;
    frameBytes.append(this->startOfFrame);
    frameBytes.append(this->length);
    frameBytes.append(this->type);
    frameBytes.append(this->APIcommandId);
    frameBytes.append(this->APIparameters);    
    frameBytes.append(this->checksum);

    return frameBytes;

}

void SramDataFrame::resetSramDataFrame()
{
    this->startOfFrame= 0;
    this->length = 0;
    this->type = 0;
    this->APIcommandId = 0;
    this->checksum = 0;
    this->APIparameters = QByteArray();
    this->numberOfParameters = 0;

    this->bytesAlreadyRead = QByteArray();
    this->bytesRead = 0;
    this->bytesInReadFrame = 50;
}

void SramDataFrame::clearAllFields()
{
    this->length = 2;
    this->type = 0;
    this->APIcommandId = 0;
    this->APIparameters.clear();
    this->numberOfParameters = 0;
    this->checksum = 0;
}


