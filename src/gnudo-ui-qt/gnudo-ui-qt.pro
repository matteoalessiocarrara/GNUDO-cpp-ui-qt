TEMPLATE = app
QT += widgets
TARGET = gnudo-ui-qt
INCLUDEPATH += .


unix:LIBS += -L../gnudo-backend/src -lbackend
unix:LIBS += -L../gnudo-backend/lib/sqlite3pp/src -lsqlite3pp
unix:LIBS += -L../gnudo-backend/lib/sqlite3pp/lib/sqlite3/ -lsqlite3
unix:LIBS += -ldl
unix:!android:LIBS += -lpthread


HEADERS += delete_priority_dialog.hpp \
           edit_priority_dialog.hpp \
           main_window.hpp \
           priority_levels_dialog.hpp \
           task_info_dialog.hpp
FORMS += delete_priority_dialog.ui \
         edit_priority_dialog.ui \
         main_window.ui \
         priority_levels_dialog.ui \
         task_info_dialog.ui
SOURCES += delete_priority_dialog.cpp \
           edit_priority_dialog.cpp \
           main.cpp \
           main_window.cpp \
           priority_levels_dialog.cpp \
           task_info_dialog.cpp
