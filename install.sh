#!/bin/sh

set -e

if command -v dnf >/dev/null 2>/dev/null; then
	sudo dnf install -y \
		translate-shell \
		make \
		cmake \
		extra-cmake-modules \
		qt6-qtbase-devel \
		kf6-krunner-devel \
		kf6-ki18n-devel \
		kf6-kcoreaddons-devel \
		kf6-kconfig-devel \
		kf6-kconfigwidgets-devel \
		kf6-kcmutils-devel
elif command -v apt >/dev/null 2>/dev/null; then
	export DEBIAN_FRONTEND=noninteractive
	sudo apt update -y
	sudo apt install -y \
		translate-shell \
		build-essential \
		cmake \
		extra-cmake-modules \
		qt6-base-dev \
		libkf6runner-dev \
		libkf6i18n-dev \
		libkf6coreaddons-dev \
		libkf6config-dev \
		libkf6configwidgets-dev \
		libkf6kcmutils-dev
else
	echo 'WARNING: Not running on Fedora or Ubuntu; dependencies may not be satisfied.'
fi

cmake -B build
cmake --build build --parallel --config Release
sudo cmake --install build --config Release
killall krunner || true
