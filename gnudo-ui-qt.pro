TEMPLATE = subdirs
SUBDIRS = backend ui

backend.subdir = lib/gnudo-backend
ui.subdir = src

ui.depends = backend