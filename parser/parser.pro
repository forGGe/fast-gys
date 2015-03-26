#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T22:53:35
#
#-------------------------------------------------

QT       += sql xml

QT       -= gui

TARGET = parser
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++11 console

# UGLY
# will be removed, when some dependecies will be placed outside core
INCLUDEPATH += ../core

SOURCES += \
    textfileparser.cpp

HEADERS += \
    parser.h \
    textfileparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
