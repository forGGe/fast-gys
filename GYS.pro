#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T00:59:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GYS
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
    gyscontroller.cpp

HEADERS  += mainwindow.h \
    gyscontroller.h \
    gystypes.h

FORMS    += mainwindow.ui
