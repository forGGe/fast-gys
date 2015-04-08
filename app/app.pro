#-------------------------------------------------
#
# Project created by QtCreator 2015-03-25T22:47:13
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = app
TEMPLATE = app

# Check if the config file exists
! include( ../common.pri ) {
    error( "Couldn't find the common.pri file!" )
}
SOURCES += \
    main.cpp \
    mainwindow.cpp \
    mainclass.cpp \

HEADERS  += \
    mainwindow.h \
    mainclass.h \


FORMS    += mainwindow.ui

# Application dependencies

# parser -----------------------------------------------------------------------
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../parser/release/ -lparser
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../parser/debug/ -lparser
else:unix: LIBS += -L$$OUT_PWD/../parser/ -lparser

INCLUDEPATH += $$PWD/../parser
DEPENDPATH += $$PWD/../parser

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../parser/release/libparser.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../parser/debug/libparser.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../parser/release/parser.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../parser/debug/parser.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../parser/libparser.a

# core -------------------------------------------------------------------------
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

# fetcher ----------------------------------------------------------------------
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../fetcher/release/ -lfetcher
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../fetcher/debug/ -lfetcher
else:unix: LIBS += -L$$OUT_PWD/../fetcher/ -lfetcher

INCLUDEPATH += $$PWD/../fetcher
DEPENDPATH += $$PWD/../fetcher

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fetcher/release/libfetcher.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fetcher/debug/libfetcher.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fetcher/release/fetcher.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../fetcher/debug/fetcher.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../fetcher/libfetcher.a
