#-------------------------------------------------
#
# Project created by QtCreator 2015-05-21T21:58:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GraphAlgorithms
TEMPLATE = app


SOURCES += main.cpp \
        mainwindow.cpp \
        graphview.cpp \
        graphnode.cpp \
        graphedge.cpp \
        parser.cpp \
        movenodecommand.cpp \
    addnodecommand.cpp \
    removenodecommand.cpp \
    removeedgecommand.cpp \
    addedgecommand.cpp \
    changeedgedirectioncommand.cpp \
    changeedgeweightcommand.cpp \
    changenodelabelcommand.cpp

HEADERS  += mainwindow.h \
            graphview.h \
            graphnode.h \
            graphedge.h \
            parser.h \
            movenodecommand.h \
    addnodecommand.h \
    removenodecommand.h \
    removeedgecommand.h \
    addedgecommand.h \
    changeedgedirectioncommand.h \
    changeedgeweightcommand.h \
    changenodelabelcommand.h

FORMS    += mainwindow.ui

RESOURCES = icons/icons.qrc
