#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T17:59:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets xml

TARGET = StateGraph
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    state.cpp \
    stategraph.cpp \
    testclass.cpp \
    stateiterator.cpp \
    superstate.cpp \
    stateexception.cpp \
    wrongindexstateexception.cpp \
    unknownstateexception.cpp \
    paintwidget.cpp \
    screendata.cpp \
    badxmlexception.cpp

HEADERS  += mainwindow.h \
    state.h \
    stategraph.h \
    simplelogger.h \
    testclass.h \
    stateiterator.h \
    superstate.h \
    stateexception.h \
    wrongindexstateexception.h \
    unknownstateexception.h \
    paintwidget.h \
    screendata.h \
    badxmlexception.h

FORMS    += mainwindow.ui
