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

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}

# UGLY
# will be removed, when some dependecies will be placed outside core
INCLUDEPATH += ../core

SOURCES += \
    textfileparser.cpp \
    rankXMLparser.cpp

HEADERS += \
    parser.h \
    textfileparser.h \
    rankXMLparser.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

