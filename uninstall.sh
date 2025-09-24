#!/bin/sh

set -e

sudo cmake --build build --target uninstall
killall krunner || true
