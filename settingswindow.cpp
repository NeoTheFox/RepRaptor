#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);
    ui->flowcontrolbox->addItem(tr("No control"));
    ui->flowcontrolbox->addItem(tr("Hardware control"));
    ui->flowcontrolbox->addItem(tr("Software control"));

    //bool firstrun = !settings.value("core/firstrun").toBool(); //firstrun is inverted!

    settings.setParent(this);
    ui->flowcontrolbox->setCurrentIndex(settings.value("core/flowcontrol", 0).toInt());
    ui->senderbox->setValue(settings.value("core/senderinterval", 2).toInt());
    ui->traybox->setChecked(settings.value("core/trayiconenabled", 1).toBool());
    ui->statusbox->setValue(settings.value("core/statusinterval", 2000).toInt());
    ui->bedxbox->setValue(settings.value("printer/bedx", 200).toInt());
    ui->bedybox->setValue(settings.value("printer/bedy", 200).toInt());
    ui->feedrateBox->setValue(settings.value("printer/feedrate", 1500).toInt());
    ui->extruderFeedrateBox->setValue(settings.value("printer/extruderfeedrate", 200).toInt());
    ui->lockbox->setChecked(settings.value("core/lockcontrols", 0).toBool());
    ui->checksumbox->setChecked(settings.value("core/checksums", 0).toBool());
    ui->sdbox->setChecked(settings.value("core/checksdstatus", 1).toBool());
    ui->dtrbox->setChecked(settings.value("core/dtr", 1).toBool());
    ui->supresswaitbox->setChecked(settings.value("user/supresswait").toBool());
    ui->bufferbox->setValue(settings.value("core/logbuffersize", 1000).toInt());

    ui->firmwarecombo->addItem("Marlin"); //0
    ui->firmwarecombo->addItem("Repetier"); //1
    ui->firmwarecombo->addItem("Other"); //2

    ui->firmwarecombo->setCurrentIndex(settings.value("printer/firmware", OtherFirmware).toInt());

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
    settings.setValue("core/flowcontrol", ui->flowcontrolbox->currentIndex());
    settings.setValue("core/trayiconenabled", ui->traybox->isChecked());
    settings.setValue("core/senderinterval", ui->senderbox->value());
    settings.setValue("core/statusinterval", ui->statusbox->value());
    settings.setValue("printer/bedy", ui->bedybox->value());
    settings.setValue("printer/bedx", ui->bedxbox->value());
    settings.setValue("printer/feedrate", ui->feedrateBox->value());
    settings.setValue("printer/extruderfeedrate", ui->extruderFeedrateBox->value());
    settings.setValue("core/lockcontrols", ui->lockbox->isChecked());
    settings.setValue("core/checksums", ui->checksumbox->isChecked());
    settings.setValue("core/checksdstatus", ui->sdbox->isChecked());
    settings.setValue("core/dtr", ui->dtrbox->isChecked());
    settings.setValue("printer/firmware", ui->firmwarecombo->currentIndex());
    settings.setValue("user/supresswait", ui->supresswaitbox->isChecked());
    settings.setValue("core/logbuffersize", ui->bufferbox->value());

    emit updatesettings();
}
