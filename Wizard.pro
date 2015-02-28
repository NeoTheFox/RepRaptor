#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T16:14:20
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wizard
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp

HEADERS  += mainwindow.h \
    settingswindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui

RESOURCES += \
    settings.qrc
