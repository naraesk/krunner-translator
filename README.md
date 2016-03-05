# Translator for KDE KRunner (Plasma 5)

This is a plugin for Plasma 5 KRunner. It's a translator and it translates text.

## Installation ##
```
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=`kf5-config --prefix` -DQT_PLUGIN_INSTALL_DIR=`kf5-config --qt-plugins`
make 
sudo make install
kquitapp krunner
krunner
```
## Configuration ##

The translation of sentences is done via [yandex](https://translate.yandex.com/). Therefore an api key is required. You have to obtain a key yourself by following these steps:  
1. Register at https://passport.yandex.com/  
2. Get a API key at https://tech.yandex.com/keys/get/?service=trnsl   
3. Copy the key to the configuration dialog  

If you do not provide an api key only the translation of single words will work.

## Usage ##

A list of all language codes you can find [here](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes).

### Specify source and target language ###
Syntax: \<sourcelang\>-\<targetlang\> \<searchterm\>  
*en-de house* → will translate *house* into german (*de*)

### Use default source language ###
Syntax: \<targetlang\> \<searchterm\>

Defuault source language: *English (en)*  
Alternative source language: *German (de)*  

*de home* → *en-de home*  
*en blau* → *de-en blau*  
*fr maison* → *en-fr maison*  

Thanks for your feedback and kudos! If you like the runner, please vote for it [here](http://kde-apps.org/content/show.php?content=156498).
