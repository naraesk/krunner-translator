#!/bin/bash

set -e

mkdir -p build
cd build

cmake .. -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` -DKDE_INSTALL_QTPLUGINDIR=`kf5-config --qt-plugins` -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

sudo make install

kquitapp5 krunner 2> /dev/null
