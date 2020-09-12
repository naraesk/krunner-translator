# Translator for KDE KRunner (Plasma 5)

![Screenshot Runner](../../wiki/screenshot/interface.png)

This is a plugin for Plasma 5 KRunner. It's a translator and it translates text. Currently [Google Translate](https://translate.google.com/), [Bing Translator](https://www.bing.com/translator), [glosbe](https://glosbe.com/), [yandex](https://www.yandex.ru/), [youdao](http://fanyi.youdao.com/), and [Baidu Fanyi](https://fanyi.baidu.com/) are supported. By clicking on the corresponding icon, the translation can be copied or read aloud.

## Packages

[![Logo Arch Linux](../../wiki/logos/arch_linux.png)](https://aur.archlinux.org/packages/plasma-runners-translator/)
[![Logo Ubuntu](../../wiki/logos/ubuntu.png)](https://github.com/naraesk/krunner-translator/releases/download/v1.4.0/plasma-runners-translator_1.4.0-1.deb)


## Manual Installation ##

1. Install [Translate Shell](https://github.com/soimort/translate-shell)
2. Install the dependencies listed below 
3. Run `./install.sh`

### Debian/Ubuntu  
`sudo apt install cmake extra-cmake-modules build-essential libkf5runner-dev libkf5textwidgets-dev qtdeclarative5-dev gettext`

### openSUSE
`sudo zypper install cmake extra-cmake-modules libQt5Widgets5 libQt5Core5 libqt5-qtlocation-devel ki18n-devel ktextwidgets-devel 
kservice-devel krunner-devel gettext-tools kconfigwidgets-devel`

## Fedora  
`sudo dnf install cmake extra-cmake-modules kf5-ki18n-devel kf5-kservice-devel kf5-krunner-devel kf5-ktextwidgets-devel gettext`

## Configuration ##

For being able to use yandex, an api key is required. You have to obtain a key yourself by following these steps:  
1. Register at https://passport.yandex.com/  
2. Get an API key at https://tech.yandex.com/keys/get/?service=trnsl   
3. Copy the key to the configuration dialog  

For users who use Youdao Translation:
1. Register at https://ai.youdao.com/
2. Get an API key and an API secret from Application Manager
3. Copy the key to configuration dialog

Baidu users:
1. Register at http://api.fanyi.baidu.com/
2. Get an API key and an API secret from Consoles
3. Copy the key to configuration dialog

If you do not provide an api key only the translation via glosbe will work.

## Usage ##

A list of all language codes you can find [here](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes).

### Specify source and target language ###
Syntax: \<sourcelang\>-\<targetlang\> \<searchterm\>  
*en-de house* → will translate *house* into german (*de*)

### Use default source language ###
Syntax: \<targetlang\> \<searchterm\>

Default source language: *English (en)*  
Alternative source language: *German (de)*  

*de soccer* → *en-de soccer*  
*en blau* → *de-en blau*  
*fr house* → *en-fr house*  

Thanks for your feedback and kudos! If you like the runner, please vote for it [here](http://kde-apps.org/content/show.php?content=156498).
  
