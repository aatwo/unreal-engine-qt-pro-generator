#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T20:10:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnrealEngineQtCreatorHelper
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    filepathedit.cpp \
    settings.cpp \
    directorysetup.cpp \
    stringutils.cpp \
    global.cpp \
    prepareprouserfile.cpp \
    unrealinstallationsetup.cpp \
    openprofilepage.cpp

HEADERS += \
        mainwindow.h \
    filepathedit.h \
    common.h \
    settings.h \
    directorysetup.h \
    stringutils.h \
    global.h \
    prepareprouserfile.h \
    unrealinstallationsetup.h \
    openprofilepage.h

FORMS += \
    directorysetup.ui \
    prepareprouserfile.ui \
    unrealinstallationsetup.ui \
    openprofilepage.ui

RESOURCES += \
    resources.qrc
