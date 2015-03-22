#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QSettings>
#include "repraptor.h"

using namespace RepRaptor;

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = 0);

    QSettings settings;

    ~SettingsWindow();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingsWindow *ui;

signals:
    void updatesettings();

};

#endif // SETTINGSWINDOW_H
