#include "errorwindow.h"
#include "ui_errorwindow.h"

using namespace RepRaptor;

ErrorWindow::ErrorWindow(QWidget *parent, QString errorText, int errType):
    QDialog(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    switch(errType)
    {
    case SerialPortError:
        ui->label->setText(tr("Serial port error:"));
        break;
    case OpenFileError:
        ui->label->setText(tr("File open error:"));
        break;
    case HardwareFailure:
        ui->label->setText(tr("Hardware failure:"));
        break;
    default:
        ui->label->setText(tr("Unknown error type:"));
        break;
    }

    ui->errorlabel->setText(errorText);
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}
