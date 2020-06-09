#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QList>
#include <QThread>
#include <QDebug>
#include <QDateTime>

#include "serialportreader.h"

#define STATE "state"
#define STATE_STANDBY "-s"
#define STATE_OUTPUT_DISABLE "-d"
#define STATE_OPERATIONAL "-o"

#define READ "read"
#define WRITE "write"
#define MAX_BYTES_TO_READWRITE 256 // 0 - 255; 256 - 511 etc.

#define END_FRAME "\r\n"

#define MEMORY_WORDS 131072
#define WORD_BIT_SIZE 8





QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void addToLogs(QString message);
    void addToFramesLog(QString frame, bool readwrite);

    void setUartPowerText(QString text, QColor color=QColor(255,0,0));
    void setMemoryPowerText(QString text, QColor color=QColor(255,0,0));
    void setLastOperationText(QString text, QColor color=QColor(0,0,0));

    void sendMessageToDevice(QByteArray messageBytes);

private slots:
    void dataFromReader(QString serialData);
    //Uart Connection tab
    void on_pushButtonSearch_clicked();
    void on_pushButtonConnect_clicked();
    void on_pushButtonDisconnect_clicked();

    //Basic testing tab
    void on_pushButtonPower_clicked();
    void on_pushButtonWriteFull_clicked();
    void on_pushButtonReadFull_clicked();

    //Advanced testing tab
    void on_pushButtonStatus_clicked();    
    void on_pushButtonWrite_clicked();
    void on_pushButtonRead_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *uartDevice;
    MySerialReader *serialReader;
    QThread *readingThread;
    int receivedFrameNumber;
    int transmittedFrameNumber;

    bool uartStatus;
    bool memoryPower;
    QString memoryStatus;

    void guiInitialize();
    void testStandInit();

    QString getOperationAddress();
    QString formatAsciiBytes(QString asciiBytes, int targetLength=0);
};
#endif // MAINWINDOW_H
