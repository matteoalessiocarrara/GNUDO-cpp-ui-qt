#-------------------------------------------------
#
# Project created by QtCreator 2016-07-03T11:32:29
#
#-------------------------------------------------

QMAKE_CXXFLAGS += -std=c++11
QT             += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gnudo-cpp-ui-qt
TEMPLATE = app


SOURCES += main.cpp\
    delete_priority_dialog.cpp \
    edit_priority_dialog.cpp \
    main_window.cpp \
    priority_levels_dialog.cpp \
    task_info_dialog.cpp

HEADERS  += \
    delete_priority_dialog.hpp \
    edit_priority_dialog.hpp \
    priority_levels_dialog.hpp \
    main_window.hpp \
    task_info_dialog.hpp

FORMS    += \
    delete_priority_dialog.ui \
    edit_priority_dialog.ui \
    main_window.ui \
    priority_levels_dialog.ui \
    task_info_dialog.ui


unix|win32: LIBS += -L/usr/local/lib -lsqlite3pp
unix|win32: LIBS += -L/usr/local/lib -lgnudo-backend


