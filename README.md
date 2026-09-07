# Translator for KDE KRunner (Plasma 6)

![Screenshot Runner](../../wiki/screenshot/interface.png)

This is a plugin for Plasma 6 KRunner. It's a translator and it translates text. By default it translates through a **local llama.cpp server** running `tencent/Hy-MT2` (see `llama-server.sh`), with [Google Translate](https://translate.google.com/), [Bing Translator](https://www.bing.com/translator), [youdao](http://fanyi.youdao.com/), and [Baidu Fanyi](https://fanyi.baidu.com/) available as optional engines. By clicking on the corresponding icon, the translation can be copied or read aloud.

## Tested scope

The local llama.cpp engine was developed and tested on **Fedora 44** (x86_64) with **KDE Plasma 6.7.4**; other distros and Plasma versions are not guaranteed.

## Packages

[![Logo Arch Linux](../../wiki/logos/arch_linux.png)](https://aur.archlinux.org/packages/plasma6-runners-translator)

Pre-built packages for recent distro releases (ubuntu, debian, fedora, opensuse) are available on the [GitHub Releases page](https://github.com/naraesk/krunner-translator/releases).

## Install Packages

`translate-shell` is **optional**: it is only used by the online engines (the key-less Google engine, Bing) and for audio playback ("Play audio"). The default local llama.cpp engine does not need it, so the plugin works fully without `translate-shell`.

### Ubuntu 25.04+

`translate-shell` (only needed for Google/Bing/audio) is packaged in Ubuntu `multiverse`. Ubuntu usually enables this by default, but enabling it explicitly keeps the recommended install reliable:

```sh
sudo add-apt-repository multiverse
sudo apt update
sudo apt install ./krunner-translator-ubuntu-amd64.deb
```

Replace the file name with the package you downloaded from the release page.

### Debian trixie

The plugin package installs directly. `translate-shell` (only needed for the Google/Bing engines and audio) is available from Debian `contrib` only, which is not enabled in stock sources. If you want Google/Bing/audio support, enable `contrib` first:

```sh
sudo editor /etc/apt/sources.list.d/debian.sources
```

Add `contrib` to each `Components:` line, then run:

```sh
sudo apt update
sudo apt install ./krunner-translator-debian-amd64.deb
```

See the Debian sources format reference for the exact file structure: <https://wiki.debian.org/SourcesList>

### Fedora 42

```sh
sudo dnf install ./krunner-translator-fedora-amd64.rpm
```

### openSUSE Tumbleweed

```sh
sudo zypper install ./krunner-translator-opensuse-amd64.rpm
```

Replace the file names above with the package matching your distro and architecture.

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
build-essential cmake extra-cmake-modules qt6-base-dev libkf6runner-dev libkf6i18n-dev libkf6coreaddons-dev libkf6config-dev libkf6configwidgets-dev libkf6kcmutils-dev
```

(`translate-shell` is not a build dependency — it is only used at runtime by the Google/Bing engines and audio playback.)

### Fedora/openSUSE Dependencies

```
cmake extra-cmake-modules cmake(Qt6Core) cmake(Qt6Gui) cmake(Qt6Widgets) cmake(Qt6Network) cmake(KF6Runner) cmake(KF6I18n) cmake(KF6CoreAddons) cmake(KF6Config) cmake(KF6ConfigWidgets) cmake(KF6KCMUtils)
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

## Local engine (llama.cpp, default)

The default engine translates through a local [llama.cpp](https://github.com/ggml-org/llama.cpp) server running `tencent/Hy-MT2-1.8B-Q4_K_M.gguf` on `127.0.0.1:8080` by default (the port is configurable; the server can be shared with other tools). Any GGUF model you load is served as-is, and the engine builds a dynamic prompt ("Translate the following text into <target language>…"), so result quality depends on the model.

- `./llama-server.sh start` — start the server if it isn't already answering.
- `./llama-server.sh stop` / `status` / `test` — manage and probe it.
- The plugin calls `start` automatically on the first translation, so nothing needs to run manually after boot.
- Log file: `/tmp/krunner-translator-llama.log`.

### Languages

The local engine accepts **every language pair the runner knows** (the full ISO code list in *Usage* above) and builds its prompt dynamically. Whether a pair actually translates well depends on the loaded model: the default `Hy-MT2` officially supports [33 languages](https://huggingface.co/tencent/Hy-MT2) (Chinese, English, French, Portuguese, Spanish, Japanese, Turkish, Russian, Arabic, Korean, Thai, Italian, German, Vietnamese, Malay, Indonesian, Filipino, Hindi, Polish, Czech, Dutch, Khmer, Burmese, Persian, Gujarati, Urdu, Telugu, Marathi, Hebrew, Bengali, Tamil, Ukrainian, Tibetan — plus traditional Chinese). Pairs outside that list are still sent to the model and usually produce a result, but quality is not guaranteed. If you need reliable translation of a specific pair, pick a GGUF model that covers it (or use an online engine).

### Local engine configuration

The "llama.cpp (local)" tab in the configuration dialog (System Settings → KRunner → Translator) overrides the script defaults on a per-machine basis:

| Field | Default | Meaning |
|-------|---------|---------|
| Model (.gguf) | `<llama.cpp dir>/models/Hy-MT2-1.8B-Q4_K_M.gguf` | path to the GGUF model file |
| llama.cpp dir | empty: `llama-server` from `PATH` | directory containing the server binary, searched **without recursion**: `<dir>/llama-server` (extracted release / prebuilt drop-in), then `<dir>/build/bin/llama-server` (cmake build tree) |
| Port | `8080` | port the local server listens on (can be shared with other tools) |
| GPU layers | `99` | layers offloaded to the GPU (`99` = all, `0` = CPU only) |

**Test server…** checks the server with the current values and starts it if needed; the first run loads the model, which can take a while.

A **system-wide llama.cpp install** (binary in `PATH`, libraries in the system library path) works with all fields left empty. When a directory is set, the binary's own directory is added to `LD_LIBRARY_PATH` (release/build trees keep the shared libraries next to the binary; system installs use the normal loader).
