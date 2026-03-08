#!/bin/bash

set -e

mkdir -p build
cd build

cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

sudo make install

kquitapp6 krunner 2> /dev/null
