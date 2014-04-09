#-------------------------------------------------
#
# Project created by QtCreator 2014-04-09T16:15:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QHotkey
TEMPLATE = app

RC_FILE = resource.rc


SOURCES += main.cpp\
        mainwindow.cpp \
    keybinder.cpp \
    modifiercombobox.cpp \
    volumepopup.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    keybinder.h \
    modifiercombobox.h \
    volumepopup.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    volumepopup.ui \
    aboutdialog.ui

win32: LIBS += -lOle32

RESOURCES += \
    images.qrc
