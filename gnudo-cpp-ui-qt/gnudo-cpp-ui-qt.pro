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
        mainwindow.cpp \
    taskinfodialog.cpp \
    prioritylevelsdialog.cpp \
    editprioritydialog.cpp \
    deleteprioritydialog.cpp

HEADERS  += mainwindow.h \
    taskinfodialog.h \
    prioritylevelsdialog.hpp \
    editprioritydialog.hpp \
    deleteprioritydialog.hpp

FORMS    += mainwindow.ui \
    taskinfodialog.ui \
    prioritylevelsdialog.ui \
    editprioritydialog.ui \
    deleteprioritydialog.ui


unix|win32: LIBS += -L/usr/local/lib -lsqlite3pp
unix|win32: LIBS += -L/usr/local/lib -lgnudo-cpp-dbdriver-abstract
unix|win32: LIBS += -L/usr/local/lib -lgnudo-cpp-dbdriver-sqlite



