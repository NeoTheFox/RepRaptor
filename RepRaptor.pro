#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T16:14:20
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepRaptor
TEMPLATE = app
CONFIG += static


SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp \
    aboutwindow.cpp

HEADERS  += mainwindow.h \
    settingswindow.h \
    aboutwindow.h

FORMS    += mainwindow.ui \
    settingswindow.ui \
    aboutwindow.ui

RESOURCES += \
    graphics.qrc

DISTFILES += \
    LICENCE \
    README.md
