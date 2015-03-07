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
        str.remove("EPR:"); // Clear the unneeded part

        repetierEEPROMline currentLine; //Storage for EEPROM values

        QStringList tmp = str.split(' ');

        currentLine.T = tmp.at(0).toInt();
        currentLine.P = tmp.at(1).toInt();
        currentLine.S = tmp.at(2);

        lines.append(currentLine);

        QString msg;
        for(int i = 3; i < tmp.size(); i++) msg+=(tmp.at(i) + " "); //Rejoin the rest

        QLayout *line = new QGridLayout();

        QLabel *label = new QLabel(msg, this);
        QLineEdit *edit = new QLineEdit(currentLine.S,this);

        QFrame* hline = new QFrame();
        hline->setFrameShape(QFrame::HLine);
        hline->setFrameShadow(QFrame::Sunken);
        line->addWidget(hline);

        edit->setObjectName("e"+QString::number(j)); //Name the LineEdit, so when it emits signal we know where it came from

        QRegExpValidator *doublevalidator = new QRegExpValidator(
                                                QRegExp("^\\-?\\d+\\.?\\d+(e\\-?\\d+)?$",
                                                Qt::CaseInsensitive), this);
        doublevalidator->setLocale(QLocale::English);

        switch(currentLine.T) // set right validator for the line
        {
        case 0:
            edit->setValidator(new QIntValidator(-128, 255, this));
        case 1:
        case 2:
            edit->setValidator(new QIntValidator(this));
            break;
        case 3:
            edit->setValidator(doublevalidator);
            break;
        default:
            break;
        }

        connect(edit, SIGNAL(textChanged(QString)), this, SLOT(lineChanged(QString)));

        line->addWidget(label);
        line->addWidget(edit);

        line->setMargin(2);

        layout->addItem(line);

        j++; // increase counter
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
    QStringList gcode;
    for(int i=0; i < changed.size(); i++)
    {
        if(changed.at(i))
        {
            QString command;

            command+=QString("M206");
            command+=QString("T");
            command+=QString::number(lines.at(i).T);
            command+=QString("P");
            command+=QString::number(lines.at(i).P);
            if(lines.at(i).T == 3) command+=QString("X");
            else command+=QString("S");
            command+=lines.at(i).S;

            gcode.append(command);
        }
    }

    emit changesComplete(gcode);
}
