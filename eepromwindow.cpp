#include "eepromwindow.h"
#include "ui_eepromwindow.h"

EEPROMWindow::EEPROMWindow(QStringList eepromLines, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EEPROMWindow)
{
    ui->setupUi(this);

    QLayout *layout = new QVBoxLayout();

    int j = 0;
    foreach (QString str, eepromLines)
    {
        j++;
        str.remove("EPR:");

        repetierEEPROMline currentLine;

        QStringList tmp = str.split(' ');

        currentLine.T = tmp.at(0).toInt();
        currentLine.P = tmp.at(1).toInt();
        currentLine.S = tmp.at(2);

        lines.append(currentLine);

        QString msg;
        for(int i = 3; i < tmp.size(); i++) msg+=(tmp.at(i) + " ");

        QLayout *line = new QHBoxLayout();

        QLabel *label = new QLabel(msg, this);
        QLineEdit *edit = new QLineEdit(currentLine.S,this);
        //QCheckBox *changebox = new QCheckBox("Save", this);

        //changebox->setObjectName("b"+QString::number(j));
        edit->setObjectName("e"+QString::number(j));

        switch(currentLine.T) // set right validator for the line
        {
        case 0:
        case 1:
        case 2:
            edit->setValidator(new QIntValidator(this));
            break;
        case 3:
            //edit->setValidator(new QDoubleValidator(this));
            break;
        default:
            break;
        }

        //connect(edit, SIGNAL(returnPressed()), changebox, SLOT(toggle()));
        connect(edit, SIGNAL(textChanged(QString)), this, SLOT(lineChanged(QString)));
        //connect(changebox, SIGNAL(toggled(bool)), this, SLOT(lineChanged()));

        line->addWidget(label);
        line->addWidget(edit);
        //line->addWidget(changebox);

        line->setMargin(2);

        layout->addItem(line);
    }
    for(int i = 0; i < lines.size(); i++) changed.append(false);
    ui->eepromWidgets->setLayout(layout);
}

EEPROMWindow::~EEPROMWindow()
{
    delete ui;
}

void EEPROMWindow::lineChanged(QString s)
{
    int num = sender()->objectName().remove(0, 1).toInt();

    lines[num].S = s;
    changed[num] = true;
}

void EEPROMWindow::on_buttonBox_accepted()
{
    for(int i=0; i < changed.size(); i++)
    {
        if(changed.at(i))
        {
            QString tmp;

            tmp+=QString("M206");
            tmp+=QString("T");
            tmp+=QString::number(lines.at(i).T);
            tmp+=QString("P");
            tmp+=QString::number(lines.at(i).P);
            if(lines.at(i).T == 3) tmp+=QString("X");
            else tmp+=QString("S");
            tmp+=lines.at(i).S;

            gcode.append(tmp);
        }
    }

    emit changesComplete(gcode);
}
