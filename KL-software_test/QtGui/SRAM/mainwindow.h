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
    void addToFramesLog(QString frame);

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
    void on_pushButtonAddress_clicked();
    void on_pushButtonWrite_clicked();
    void on_pushButtonRead_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *uartDevice;
    MySerialReader *serialReader;
    QThread *readingThread;
    int receivedFrameNumber;

    bool uartStatus;
    bool memoryPower;
    QString memoryStatus;
    QByteArray *savedAddress;

    void guiInitialize();
    void testStandInit();
};
#endif // MAINWINDOW_H
