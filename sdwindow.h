#ifndef SDWINDOW_H
#define SDWINDOW_H

#include <QDialog>

namespace Ui {
class SDWindow;
}

class SDWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SDWindow(QStringList files, QWidget *parent = 0);
    ~SDWindow();

signals:
    void fileSelected(QString filename);

private slots:
    void on_buttonBox_accepted();

    void on_fileslist_doubleClicked(const QModelIndex &);

private:
    Ui::SDWindow *ui;
};

#endif // SDWINDOW_H
