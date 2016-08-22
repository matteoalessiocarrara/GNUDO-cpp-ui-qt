#!/bin/bash
#
#  Copyright 2016 Matteo Alessio Carrara <sw.matteoac@gmail.com>
#
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 2 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
#  MA 02110-1301, USA.
#

set -e -u

SQLITE3PP_VERSION="0.1"
BACKEND_VERSION="0.1"
UI_VERSION="0.3"

SQLITE3PP_URL=https://github.com/matteoalessiocarrara/sqlite3pp/archive/$SQLITE3PP_VERSION.zip
BACKEND_URL=https://github.com/matteoalessiocarrara/GNUDO-backend/archive/$BACKEND_VERSION.zip
UI_URL=https://github.com/matteoalessiocarrara/GNUDO-cpp-ui-qt/archive/$UI_VERSION.zip


mkdir /tmp/gnudo
cd /tmp/gnudo


# Sqlite3pp
wget $SQLITE3PP_URL -O sqlite3pp-$SQLITE3PP_VERSION.zip
unzip sqlite3pp-$SQLITE3PP_VERSION.zip
cd sqlite3pp-$SQLITE3PP_VERSION
cmake .
make
sudo make install
cd ..


# Backend
wget $BACKEND_URL -O backend-$BACKEND_VERSION.zip
unzip backend-$BACKEND_VERSION.zip
cd GNUDO-backend-$BACKEND_VERSION
cmake .
make
sudo make install
cd ..


# Qt gui
wget $UI_URL -O ui-$UI_VERSION.zip
unzip ui-$UI_VERSION.zip
cd GNUDO-cpp-ui-qt-$UI_VERSION
mkdir build
cd build
qmake ../gnudo-cpp-ui-qt
make
sudo cp gnudo-cpp-ui-qt /usr/local/bin


rm -rf /tmp/gnudo
echo "\n\n\nInstallazione completata! Adesso dovresti poter eseguire gnudo con il comando gnudo-cpp-ui-qt"

