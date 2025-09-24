#!/bin/sh

sudo cmake --build build --target uninstall
killall krunner || true
