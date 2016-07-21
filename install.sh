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
wget https://github.com/matteoalessiocarrara/sqlite3pp/archive/1aeb21cf7339fbe7313b83e42563c50c38965b40.zip
unzip 1aeb21cf7339fbe7313b83e42563c50c38965b40
cd sqlite3pp-1aeb21cf7339fbe7313b83e42563c50c38965b40
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Abstract driver
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-dbdriver-abstract/archive/alpha1.zip -O abstract.zip
unzip abstract.zip
cd GNUDO-cpp-dbdriver-abstract-alpha1
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Sqlite driver
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-dbdriver-sqlite/archive/alpha1.zip -O sqlite.zip
unzip sqlite.zip
cd GNUDO-cpp-dbdriver-sqlite-alpha1
mkdir build
cd build
cmake ..
make
sudo make install
cd ../..


# Qt gui
wget https://github.com/matteoalessiocarrara/GNUDO-cpp-ui-qt/archive/alpha1.zip -O qt-gui.zip
unzip qt-gui.zip
cd GNUDO-cpp-ui-qt-alpha1
mkdir build
cd build
qmake ../gnudo-cpp-ui-qt
make
sudo cp gnudo-cpp-ui-qt /usr/local/bin

rm -rf /tmp/gnudo

echo "\n\n\nInstallazione completata! Adesso dovresti poter esseguire gnudo con il comando gnudo-cpp-ui-qt"

