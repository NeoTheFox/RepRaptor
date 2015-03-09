#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QtSerialPort/QtSerialPort>
#include <QFile>
#include <QFuture>
#include <QVector>
#include <QTextStream>
#include <QtConcurrent/QtConcurrent>
#include <QSettings>
#include <QElapsedTimer>
#include <QMessageBox>
#include <QRegExp>

#include "settingswindow.h"
#include "aboutwindow.h"
#include "errorwindow.h"
#include "sdwindow.h"
#include "repraptor.h"
#include "eepromwindow.h"
#include "parser.h"

using namespace RepRaptor;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Parser *parser;
    QThread *parserThread;

protected:
    QFile gfile;
    QVector<QString> gcode;
    QQueue <QString> userCommands;
    QTimer sendTimer;
    QTimer progressSDTimer;
    QTimer statusTimer;
    QElapsedTimer sinceLastTemp;
    QSettings settings;
    QStringList recentFiles;
    QStringList EEPROMSettings;

private:
    Ui::MainWindow *ui;

    void parseFile(QFile &file);
    QSerialPort printer;
    QSerialPortInfo printerinfo;
    bool firstrun;
    bool autolock;
    bool sending;
    bool paused;
    bool checkingTemperature;
    bool readingFiles;
    bool sdprinting;
    bool echo;
    bool sendingChecksum;
    bool chekingSDStatus;
    int firmware;
    long int currentLine;
    unsigned long int lastRecieved;
    int readyRecieve;
    unsigned long int sdBytes;

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
    void initSDprinting(QStringList sdFiles);
    void selectSDfile(QString file);
    void checkSDStatus();
    void updateSDStatus(SDProgress p);
    void requestEEPROMSettings();
    void openEEPROMeditor();
    void sendEEPROMsettings(QStringList changes);
    void updateTemperature(TemperatureReadings r);
    void EEPROMSettingRecieved(QString esetting);
    void recievedOkNum(int num);
    void recievedWait();
    void recievedError();
    void recievedSDDone();
    void recievedResend(int num);

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
    void on_haltbtn_clicked();
    void on_actionAbout_Qt_triggered();
    void on_speedslider_valueChanged(int value);
    void on_speededit_textChanged(const QString &arg1);
    void on_speedsetbtn_clicked();
    void on_flowedit_textChanged(const QString &arg1);
    void on_flowslider_valueChanged(int value);
    void on_flowbutton_clicked();
    void on_checktemp_stateChanged(int arg1);
    void on_actionSettings_triggered();
    void on_stepspin_valueChanged(const QString &arg1);
    void on_estepspin_valueChanged(const QString &arg1);
    void on_releasebtn_clicked();
    void on_actionAbout_triggered();
    void serialError(QSerialPort::SerialPortError error);
    void on_actionPrint_from_SD_triggered();
    void on_actionSet_SD_printing_mode_triggered();
    void on_actionEEPROM_editor_triggered();

signals:
    void sdReady();
    void eepromReady();
    void recievedData(QByteArray);
    void startedReadingEEPROM();
};

#endif // MAINWINDOW_H
