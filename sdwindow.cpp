#include "sdwindow.h"
#include "ui_sdwindow.h"

SDWindow::SDWindow(QStringList files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SDWindow)
{
    ui->setupUi(this);

    ui->fileslist->addItems(files);
}

SDWindow::~SDWindow()
{
    delete ui;
}

void SDWindow::on_buttonBox_accepted()
{
    emit fileSelected(ui->fileslist->currentItem()->text());
}

void SDWindow::on_fileslist_doubleClicked(const QModelIndex &index)
{
    emit fileSelected(ui->fileslist->currentItem()->text());
    this->close();
}
