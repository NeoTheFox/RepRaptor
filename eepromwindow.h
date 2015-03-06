#ifndef EEPROMWINDOW_H
#define EEPROMWINDOW_H

#include <QDialog>
#include <QtWidgets>

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

signals:
    void changesComplete(QStringList changed);
private slots:
    void on_buttonBox_accepted();
};

#endif // EEPROMWINDOW_H
