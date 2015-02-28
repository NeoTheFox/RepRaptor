#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("NeoTheFox");
    QCoreApplication::setOrganizationDomain("https://github.com/NeoTheFox");
    QCoreApplication::setApplicationName("RepRaptor");

    MainWindow w;
    w.show();

    return a.exec();
}
