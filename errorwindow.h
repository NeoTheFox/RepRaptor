#ifndef ERRORWINDOW_H
#define ERRORWINDOW_H

#include <QDialog>

#include "erroricon.h"
#include "repraptor.h"

namespace Ui {
class ErrorWindow;
}

class ErrorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorWindow(QWidget *parent = 0, QString errorText = "Unknown error", int errType = 0);
    ~ErrorWindow();

private:
    Ui::ErrorWindow *ui;
};

#endif // ERRORWINDOW_H
