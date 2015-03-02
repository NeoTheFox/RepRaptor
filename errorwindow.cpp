#include "errorwindow.h"
#include "ui_errorwindow.h"

ErrorWindow::ErrorWindow(QWidget *parent, QString errorText) :
    QDialog(parent),
    ui(new Ui::ErrorWindow)
{
    ui->setupUi(this);
    ui->errorlabel->setText(errorText);
}

ErrorWindow::~ErrorWindow()
{
    delete ui;
}
