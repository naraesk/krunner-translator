#!/bin/sh

set -e

shared_deps="
translate-shell
make
cmake
extra-cmake-modules
"

fedora_opensuse_deps="
cmake(Qt6Core)
cmake(Qt6Gui)
cmake(Qt6Widgets)
cmake(Qt6Network)
cmake(KF6Runner)
cmake(KF6I18n)
cmake(KF6CoreAddons)
cmake(KF6Config)
cmake(KF6ConfigWidgets)
cmake(KF6KCMUtils)
"

ubuntu_deps="
build-essential
qt6-base-dev
libkf6runner-dev
libkf6i18n-dev
libkf6coreaddons-dev
libkf6config-dev
libkf6configwidgets-dev
libkf6kcmutils-dev
"

if command -v dnf >/dev/null 2>/dev/null; then
	sudo dnf install -y $shared_deps $fedora_opensuse_deps
elif command -v zypper >/dev/null 2>/dev/null; then
	sudo zypper install -y $shared_deps $fedora_opensuse_deps
elif command -v apt >/dev/null 2>/dev/null; then
	export DEBIAN_FRONTEND=noninteractive
	sudo apt update -y
	sudo apt install -y $shared_deps $ubuntu_deps
else
	echo 'WARNING: Could not find known package manager; dependencies may not be satisfied.'
fi

cmake -B build
cmake --build build --parallel --config Release
sudo cmake --install build --config Release
killall krunner || true
