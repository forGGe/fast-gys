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
    gys_controller.cpp \
    gys_csvfetcher.cpp

HEADERS  += mainwindow.h \
    gys_controller.h \
    gys_types.h \
    gys_csvfetcher.h

FORMS    += mainwindow.ui
