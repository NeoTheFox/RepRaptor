#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    if(settings.value("core/senderinterval").toFloat()) ui->senderbox->setValue(settings.value("core/senderinterval").toFloat());
    else ui->senderbox->setValue(1);

    if(settings.value("core/statusinterval").toInt()) ui->statusbox->setValue(settings.value("core/statusinterval").toInt());
    else ui->senderbox->setValue(3000);

    if(settings.value("printer/bedx").toInt()) ui->bedxbox->setValue(settings.value("printer/bedx").toInt());
    else ui->bedxbox->setValue(200);

    if(settings.value("printer/bedy").toInt()) ui->bedybox->setValue(settings.value("printer/bedy").toInt());
    else ui->bedybox->setValue(200);

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
}
