# Translator for KDE KRunner (Plasma 6)

![Screenshot Runner](../../wiki/screenshot/interface.png)

This is a plugin for Plasma 6 KRunner. It's a translator and it translates text. Currently [Google Translate](https://translate.google.com/), [Bing Translator](https://www.bing.com/translator), [youdao](http://fanyi.youdao.com/), and [Baidu Fanyi](https://fanyi.baidu.com/) are supported. By clicking on the corresponding icon, the translation can be copied or read aloud.

## Packages

[![Logo Arch Linux](../../wiki/logos/arch_linux.png)](https://aur.archlinux.org/packages/plasma6-runners-translator)

## Building from source

1. Clone this repo, `cd` into it, and run `./install.sh`
2. To uninstall, run `./uninstall.sh`

OR, if you prefer to build manually,

1. Install the dependencies listed below
2. Clone this repo, `cd` into it, and run
```sh
cmake -B build
cmake --build build --parallel --config Release
sudo cmake --install build --config Release
killall -q krunner  # or systemctl --user restart plasma-krunner
```
3. To uninstall, run
```sh
sudo cmake --build build --target uninstall
killall -q krunner  # or systemctl --user restart plasma-krunner
```

### Ubuntu Dependencies

Note: Requires Ubuntu 25.04 (Plucky) or newer.

```
translate-shell build-essential cmake extra-cmake-modules qt6-base-dev libkf6runner-dev libkf6i18n-dev libkf6coreaddons-dev libkf6config-dev libkf6configwidgets-dev libkf6kcmutils-dev
```

### Fedora/openSUSE Dependencies

```
translate-shell cmake extra-cmake-modules cmake(Qt6Core) cmake(Qt6Gui) cmake(Qt6Widgets) cmake(Qt6Network) cmake(KF6Runner) cmake(KF6I18n) cmake(KF6CoreAddons) cmake(KF6Config) cmake(KF6ConfigWidgets) cmake(KF6KCMUtils)
```

## Pre-built packages

Pre-built packages for recent distro releases are available on the [GitHub Releases page](https://github.com/naraesk/krunner-translator/releases).

**Ubuntu 25.04:**
```sh
sudo dpkg -i krunner-translator-ubuntu.deb
```

**Fedora 42:**
```sh
sudo dnf install ./krunner-translator-fedora.rpm
```

**openSUSE Tumbleweed** (package is unsigned):
```sh
sudo zypper --no-gpg-checks install ./krunner-translator-opensuse.rpm
```

## Configuration

For being able to use Youdao and Baidu, an API key is required. You have to obtain a key yourself by following these steps:

### Youdao
1. Register at https://ai.youdao.com/
2. Get an API key and an API secret from Application Manager
3. Copy the key to configuration dialog

### Baidu
1. Register at http://api.fanyi.baidu.com/
2. Get an API key and an API secret from Consoles
3. Copy the key to configuration dialog

## Usage

A list of all language codes you can find [here](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes).

### Specify source and target language

Syntax: `<sourcelang>-<targetlang> <searchterm>`

*en-de house* will translate *house* into german (*de*)

### Use default source language

Syntax: `<targetlang> <searchterm>`

Default source language: *English (en)*
Alternative source language: *German (de)*

*de soccer* → *en-de soccer*
*en blau* → *de-en blau*
*fr house* → *en-fr house*
