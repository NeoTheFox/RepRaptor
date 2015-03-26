#include "sdwindow.h"
#include "ui_sdwindow.h"

SDWindow::SDWindow(QStringList files, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SDWindow)
{
    ui->setupUi(this);

    ui->fileslist->setSelectionMode(QListView::SingleSelection);

    if(!files.isEmpty())
    {
        ui->fileslist->addItems(files);
        ui->fileslist->setCurrentItem(ui->fileslist->itemAt(0,0));
    }
}

SDWindow::~SDWindow()
{
    delete ui;
}

void SDWindow::on_buttonBox_accepted()
{
    if(ui->fileslist->count() > 0) emit fileSelected(ui->fileslist->currentItem()->text());
}

void SDWindow::on_fileslist_doubleClicked(const QModelIndex &)
{
    if(ui->fileslist->count() > 0) emit fileSelected(ui->fileslist->currentItem()->text());
    this->close();
}
