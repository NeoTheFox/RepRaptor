#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    if(settings.value("core/senderinterval").toInt()) ui->senderbox->setValue(settings.value("core/senderinterval").toInt());
    else ui->senderbox->setValue(50);

    if(settings.value("core/statusinterval").toInt()) ui->statusbox->setValue(settings.value("core/statusinterval").toInt());
    else ui->senderbox->setValue(3000);

}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::on_buttonBox_accepted()
{
    settings.setValue("core/senderinterval", ui->senderbox->value());
    settings.setValue("core/statusinterval", ui->statusbox->value());
}
