#-------------------------------------------------
#
# Project created by QtCreator 2015-03-26T00:53:15
#
#-------------------------------------------------

QT       += network sql

QT       -= gui

TARGET = fetcher
TEMPLATE = lib
CONFIG += staticlib

QMAKE_CXXFLAGS += -Wpedantic -Wall -Wextra -Wformat
CONFIG += c++11 console

SOURCES += \
    fetcher.cpp

HEADERS += \
    filefetcher.h \
    fetcher.h
unix {
    target.path = /usr/lib
    INSTALLS += target
}

# Dependecies
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../core/release/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../core/debug/ -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/libcore.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/libcore.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/release/core.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../core/debug/core.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../core/libcore.a
