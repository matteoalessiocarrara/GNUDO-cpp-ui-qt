TEMPLATE = subdirs
SUBDIRS = backend ui

backend.subdir = src/gnudo-backend
ui.subdir = src/gnudo-ui-qt

ui.depends = backend