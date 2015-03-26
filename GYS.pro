#-------------------------------------------------
#
# Project created by QtCreator 2014-06-04T00:59:52
#
#-------------------------------------------------


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GYS
TEMPLATE = subdirs

CONFIG += ordered

SUBDIRS += \
    core \
    parser \
    fetcher \
    app \

# Use .depends to specify that a project depends on another.
app.depends += core
app.depends += parser
app.depends += fetcher

fetcher.depends += core
