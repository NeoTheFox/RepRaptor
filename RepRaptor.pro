#-------------------------------------------------
#
# Project created by QtCreator 2015-02-26T16:14:20
# Licenced on terms of GNU GPL v2 licence
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RepRaptor
TEMPLATE = app
CONFIG += static

unix {
    #VARIABLES
    isEmpty(PREFIX) {
        PREFIX = /usr
    }
    BINDIR = $$PREFIX/bin
    DATADIR =$$PREFIX/share

    DEFINES += DATADIR=\\\"$$DATADIR\\\" PKGDATADIR=\\\"$$PKGDATADIR\\\"

    #MAKE INSTALL

    INSTALLS += target desktop icon

    target.path =$$BINDIR

    desktop.path = $$DATADIR/applications
    desktop.files += $${TARGET}.desktop

    icon.path = $$DATADIR/icons
    icon.files += icons/repraptor.png
}

SOURCES += main.cpp\
        mainwindow.cpp \
    settingswindow.cpp \
    aboutwindow.cpp \
    errorwindow.cpp \
    erroricon.cpp \
    sdwindow.cpp \
    eepromwindow.cpp \
    parser.cpp \
    sender.cpp

HEADERS  += mainwindow.h \
    settingswindow.h \
    aboutwindow.h \
    errorwindow.h \
    erroricon.h \
    sdwindow.h \
    repraptor.h \
    eepromwindow.h \
    parser.h \
    sender.h

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
    RepRaptor.desktop \
    .travis.yml
