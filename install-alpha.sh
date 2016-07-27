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


mkdir /tmp/gnudo
cd /tmp/gnudo


# Sqlite3pp
sqlite3pp_version=00708e116d481e463532bc03bddf80a19c812e06
wget https://github.com/matteoalessiocarrara/sqlite3pp/archive/$sqlite3pp_version.zip
unzip $sqlite3pp_version.zip
cd sqlite3pp-$sqlite3pp_version
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Abstract driver
abstract_version=alpha2
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-dbdriver-abstract/archive/$abstract_version.zip -O abstract.zip
unzip abstract.zip
cd GNUDO-cpp-dbdriver-abstract-$abstract_version
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Sqlite driver
sqlite_driver_version=alpha2
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-dbdriver-sqlite/archive/$sqlite_driver_version.zip -O sqlite.zip
unzip sqlite.zip
cd GNUDO-cpp-dbdriver-sqlite-$sqlite_driver_version
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Qt gui
gui_version=alpha2
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-ui-qt/archive/$gui_version.zip -O qt-gui.zip
unzip qt-gui.zip
cd GNUDO-cpp-ui-qt-$gui_version
mkdir build
cd build
qmake ../gnudo-cpp-ui-qt
make
sudo cp gnudo-cpp-ui-qt /usr/local/bin

rm -rf /tmp/gnudo

echo "\n\n\nInstallazione completata! Adesso dovresti poter eseguire gnudo con il comando gnudo-cpp-ui-qt"

