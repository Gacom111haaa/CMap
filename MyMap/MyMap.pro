#-------------------------------------------------
#
# Project created by QtCreator 2016-11-13T22:27:21
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MyMap
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    cmapwidget.cpp

HEADERS  += mainwindow.h \
    cmapwidget.h

FORMS    += mainwindow.ui

RESOURCES +=
