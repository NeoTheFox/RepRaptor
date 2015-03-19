#include "mainwindow.h"
#include "repraptor.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Set some props in order to fetch and save QSettings
    QCoreApplication::setOrganizationName("NeoTheFox");
    QCoreApplication::setOrganizationDomain("https://github.com/NeoTheFox");
    QCoreApplication::setApplicationName("RepRaptor");
    QCoreApplication::setApplicationVersion(REPRAPTOR_VERSION);

    //Set the priority, so the OS would not mess up serial communication
    QThread::currentThread()->setPriority(QThread::HighestPriority);

    MainWindow w;
    w.show();

    return a.exec();
}
