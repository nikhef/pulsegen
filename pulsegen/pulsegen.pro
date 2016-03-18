#-------------------------------------------------
#
# Project created by QtCreator 2016-02-15T08:57:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pulsegen
TEMPLATE = app


SOURCES += main.cpp\
        pulsegenerator.cpp

HEADERS  += pulsegenerator.h

FORMS    += pulsegenerator.ui

RESOURCES += \
    resource.qrc

LIBS += -lwiringPi
