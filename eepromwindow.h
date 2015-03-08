#ifndef EEPROMWINDOW_H
#define EEPROMWINDOW_H

#include <QDialog>
#include <QtWidgets>

#include "repraptor.h"

using namespace RepRaptor;

namespace Ui {
class EEPROMWindow;
}

class EEPROMWindow : public QDialog
{
    Q_OBJECT

public:
    explicit EEPROMWindow(QStringList eepromLines, QWidget *parent = 0);
    ~EEPROMWindow();

private:
    Ui::EEPROMWindow *ui;
    QVector <repetierEEPROMline> lines;
    QVector <bool> changed;
    int firmware;

signals:
    void changesComplete(QStringList changed);

private slots:
    void on_buttonBox_accepted();
    void lineChanged(QString s);
};

#endif // EEPROMWINDOW_H
