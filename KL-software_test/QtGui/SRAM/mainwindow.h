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

private slots:
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

    void testStandInit();
};
#endif // MAINWINDOW_H