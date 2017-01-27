#-------------------------------------------------
#
# Project created by QtCreator 2016-09-16T17:28:17
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = game2
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    card.cpp

HEADERS  += mainwindow.h \
    card.h \
    const.h

FORMS    += mainwindow.ui

CONFIG   += c++11
