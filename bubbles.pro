#-------------------------------------------------
#
# Project created by QtCreator 2017-10-02T12:43:39
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
CONFIG += c++11
TARGET = bubbles
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    calculator/coordscalculator.cpp \
    graphics/bubblegraphicsitem.cpp \
    graphics/bubblescene.cpp

HEADERS += \
        mainwindow.h \
    calculator/coordscalculator.h \
    graphics/bubblegraphicsitem.h \
    graphics/bubblescene.h \
    locker.h \
    calculator/movementcalculator.h \
    bubble.h \
    testshapes.h

INCLUDEPATH += graphics \
                calculator

