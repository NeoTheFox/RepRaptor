#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtSerialPort/QtSerialPort>
#include <QFile>
#include <QFuture>
#include <QVector>
#include <QTextStream>
#include <QTimer>
#include <QSettings>
#include <QElapsedTimer>

#include "settingswindow.h"
#include "aboutwindow.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QFile gfile;
    QVector<QString> gcode;
    QTimer sendTimer;
    QTimer statusTimer;
    QTimer tempWarning;
    QElapsedTimer sinceLastTemp;
    QSettings settings;
    QStringList recentFiles;

private:
    Ui::MainWindow *ui;

    void parseFile(QFile &file);
    QSerialPort printer;
    QSerialPortInfo printerinfo;
    bool sending;
    bool paused;
    bool commandDone;
    bool checkingTemperature;
    bool injectingCommand;
    int currentLine;
    QString userCommand;


private slots:
    void open();
    void serialconnect();
    void serialupdate();
    bool sendLine(QString line);

    void readSerial();
    void printMsg(QString text);
    void printMsg(const char* text);
    void sendNext();
    void checkStatus();
    void updateRecent();
    void injectCommand(QString command);

    void xplus();
    void yplus();
    void zplus();
    void eplus();
    void xminus();
    void yminus();
    void zminus();
    void eminus();
    void xhome();
    void yhome();
    void zhome();
    void ezero();
    void homeall();
    void bedcenter();
    void on_sendbtn_clicked();
    void on_fanonbtn_clicked();
    void on_fanoffbtn_clicked();
    void on_atxonbtn_clicked();
    void on_atxoffbtn_clicked();
    void on_etmpset_clicked();
    void on_etmpoff_clicked();
    void on_btmpset_clicked();
    void on_btmpoff_clicked();
    void on_sendBtn_clicked();
    void on_pauseBtn_clicked();
    void on_checktemp_stateChanged(int arg1);
    void on_actionSettings_triggered();
    void on_releasebtn_clicked();
    void on_actionAbout_triggered();
};

#endif // MAINWINDOW_H
