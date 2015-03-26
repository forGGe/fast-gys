#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T00:35:34
#
#-------------------------------------------------

QT       -= gui
QT       += sql xml

TARGET = core
TEMPLATE = lib
CONFIG += staticlib

SOURCES += \
    ranks.cpp \

HEADERS += \
    types.h \
    exceptions.h \
    ranks.h \

    #controller.cpp \
    #controller.h \

QMAKE_CXXFLAGS += -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++11 console

unix {
    target.path = /usr/lib
    INSTALLS += target
}
