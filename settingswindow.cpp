#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    bool firstrun = !settings.value("core/firstrun").toBool(); //firstrun is inverted!

    if(firstrun) ui->senderbox->setValue(4);
    else ui->senderbox->setValue(settings.value("core/senderinterval").toFloat());

    if(firstrun) ui->echobox->setChecked(false);
    else ui->echobox->setChecked(settings.value("core/echo").toBool());

    if(settings.value("core/statusinterval").toInt()) ui->statusbox->setValue(settings.value("core/statusinterval").toInt());
    else ui->statusbox->setValue(1500);

    if(settings.value("printer/bedx").toInt()) ui->bedxbox->setValue(settings.value("printer/bedx").toInt());
    else ui->bedxbox->setValue(200);

    if(settings.value("printer/bedy").toInt()) ui->bedybox->setValue(settings.value("printer/bedy").toInt());
    else ui->bedybox->setValue(200);

    ui->lockbox->setChecked(settings.value("core/lockcontrols").toBool());
    ui->checksumbox->setChecked(settings.value("core/checksums").toBool());
    ui->sdbox->setChecked(settings.value("core/checksdstatus").toBool());

    ui->firmwarecombo->addItem("Marlin");
    ui->firmwarecombo->addItem("Repetier");
    ui->firmwarecombo->addItem("Other");

    if(firstrun) ui->firmwarecombo->setCurrentIndex(OtherFirmware);
    else ui->firmwarecombo->setCurrentIndex(settings.value("printer/firmware").toInt());

    #ifdef QT_DEBUG
    ui->checksumbox->setEnabled(true);
    #else
    ui->checksumbox->setDisabled(true);
    #endif

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    settings.setValue("core/senderinterval", ui->senderbox->value());
    settings.setValue("core/statusinterval", ui->statusbox->value());
    settings.setValue("printer/bedy", ui->bedybox->value());
    settings.setValue("printer/bedx", ui->bedxbox->value());
    settings.setValue("core/echo", ui->echobox->isChecked());
    settings.setValue("core/lockcontrols", ui->lockbox->isChecked());
    settings.setValue("core/checksums", ui->checksumbox->isChecked());
    settings.setValue("core/checksdstatus", ui->sdbox->isChecked());
    settings.setValue("printer/firmware", ui->firmwarecombo->currentIndex());
}
