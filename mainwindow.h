#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QSystemTrayIcon>
#include <QtSerialPort/QtSerialPort>
#include <QFile>
#include <QThread>
#include <QVector>
#include <QTextStream>
#include <QSettings>
#include <QElapsedTimer>
#include <QRegExp>
#include <QMap>

#include "settingswindow.h"
#include "aboutwindow.h"
#include "errorwindow.h"
#include "sdwindow.h"
#include "repraptor.h"
#include "eepromwindow.h"
#include "parser.h"
#include "sender.h"

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

    Parser *parserWorker;
    Sender *senderWorker;
    QThread *parserThread;
    QThread *senderThread;

protected:
    QMap <QString, QString> macros;
    QFile gfile;
    QVector<QString> gcode;
    QQueue <QString> userCommands;
    QTimer *progressSDTimer;
    QTimer *statusTimer;
    QSystemTrayIcon *trayIcon;
    QMenu *recentMenu;
    QMenu *trayIconMenu;
    QElapsedTimer *sinceLastTemp;
    QElapsedTimer *sinceLastSDStatus;
    QSettings settings;
    QStringList recentFiles;
    QStringList EEPROMSettings;
    QStringList userHistory;
    QSerialPortInfo printerinfo;
    QTextCursor terminalCursor;

    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *target, QEvent *event);

private:
    Ui::MainWindow *ui;

    QString lastDir;
    bool opened;
    bool firstrun;
    bool autolock;
    bool printing;
    bool sending;
    bool paused;
    bool checkingTemperature;
    bool sdprinting;
    bool echo;
    bool chekingSDStatus;
    bool trayIconEnabled;
    bool supressWait;
    int firmware;
    int feedrate;
    int extruderFeedrate;
    int userHistoryPos;
    unsigned long int sdBytes;

private slots:
    void open();
    void serialconnect();
    void serialupdate();
    void updatesettings();
    void readSerial(QByteArray data);
    void printMsg(QString text);
    void checkStatus();
    void updateRecent();
    void initSDprinting(QStringList sdFiles);
    void selectSDfile(QString file);
    void checkSDStatus();
    void updateSDStatus(SDProgress p);
    void requestEEPROMSettings();
    void openEEPROMeditor();
    void sendEEPROMsettings(QStringList changes);
    void updateTemperature(TemperatureReadings r);
    void EEPROMSettingReceived(QString esetting);
    void receivedError();
    void receivedSDDone();
    void receivedNotSDPrinting();
    void parseFile(QString filename);
    void recentClicked();
    void updateFileProgress(FileProgress);
    void baudrateSetFailed(int b);
    void trayIconClicked(QSystemTrayIcon::ActivationReason reason);

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
    void startedReadingEEPROM();

    void openPort(QSerialPortInfo i);
    void closePort();
    void startPrinting();
    void stopPrinting();
    void pause(bool p);
    void setBaudrate(int b);
    void setFile(QVector <QString> f);
    void injectCommand(QString command);
    void flushInjectionBuffer();
};

#endif // MAINWINDOW_H
