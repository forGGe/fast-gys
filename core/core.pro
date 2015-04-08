#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T00:35:34
#
#-------------------------------------------------

QT       -= gui
QT       += sql xml network

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

SOURCES += \

HEADERS += \
    types.h \
    exceptions.h \

    #controller.cpp \
    #controller.h \

QMAKE_CXXFLAGS += -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++11 console

unix {
    target.path = /usr/lib
    INSTALLS += target
}
