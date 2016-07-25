#-------------------------------------------------
#
# Project created by QtCreator 2016-07-01T08:57:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = snake
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamewidget.cpp

HEADERS  += mainwindow.h \
    gamewidget.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc
