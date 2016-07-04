#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T11:32:29
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnudo-cpp-ui-qt
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

unix|win32: LIBS += -lgnudo-cpp-dbdriver-sqlite
