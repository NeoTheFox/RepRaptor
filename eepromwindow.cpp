#include "eepromwindow.h"
#include "ui_eepromwindow.h"

EEPROMWindow::EEPROMWindow(QStringList eepromLines, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EEPROMWindow)
{
    ui->setupUi(this);

    QLayout *layout = new QVBoxLayout();
    foreach (QString str, eepromLines)
    {
        str.remove("EPR:");
        int T, P;
        double S;

        QStringList tmp = str.split(' ');

        T = tmp.at(0).toInt();
        P = tmp.at(1).toInt();
        S = tmp.at(2).toDouble();

        QString msg;
        for(int i = 3; i < tmp.size(); i++) msg+=(tmp.at(i) + " ");

        QLayout *line = new QHBoxLayout();

        QLabel *label = new QLabel(msg, this);
        QLineEdit *edit = new QLineEdit(QString::number(S),this);

        line->addWidget(label);
        line->addWidget(edit);

        layout->addItem(line);
    }
    ui->eepromWidgets->setLayout(layout);
}

EEPROMWindow::~EEPROMWindow()
{
    delete ui;
}

void EEPROMWindow::on_buttonBox_accepted()
{

}
