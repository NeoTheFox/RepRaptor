#include "aboutwindow.h"
#include "ui_aboutwindow.h"

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow)
{
    ui->setupUi(this);
    QString info;
    info += "RepRaptor v" + qApp->applicationVersion() + ", ";
    info += "built with Qt " + QString(QT_VERSION_STR) + " on ";
    info += QString(__DATE__) + ", " + QString(__TIME__);
    ui->versionLabel->setText(info);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}
