#!/bin/bash

# Exit immediately if something fails
set -e

cd build
sudo make uninstall
kquitapp6 krunner 2> /dev/null; kstart krunner > /dev/null 2>&1 &
