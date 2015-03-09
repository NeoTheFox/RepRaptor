#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T16:14:20
# Licenced on terms of GNU GPL v2 licence
#
#-------------------------------------------------

QT       += core gui serialport concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepRaptor
TEMPLATE = app
CONFIG += static

unix
{
    icon.files += icons/icon.png
    icon.path = /usr/local/share/icons

    desktop.files += RepRaptor.desktop
    desktop.path = /usr/local/share/applications

    repraptor.files += RepRaptor
    repraptor.extra = cp RepRaptor repraptor
    repraptir.path = /usr/local/bin

    INSTALLS+= icon desktop repraptor
}

SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp \
    aboutwindow.cpp \
    errorwindow.cpp \
    erroricon.cpp \
    sdwindow.cpp \
    eepromwindow.cpp \
    parser.cpp

HEADERS  += mainwindow.h \
    settingswindow.h \
    aboutwindow.h \
    errorwindow.h \
    erroricon.h \
    sdwindow.h \
    repraptor.h \
    eepromwindow.h \
    parser.h

FORMS    += mainwindow.ui \
    settingswindow.ui \
    aboutwindow.ui \
    errorwindow.ui \
    sdwindow.ui \
    eepromwindow.ui

RESOURCES += \
    graphics.qrc

DISTFILES += \
    LICENCE \
    README.md \
    RepRaptor.desktop
