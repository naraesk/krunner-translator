# Translator for KDE KRunner (Plasma 5)

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

## Usage ##
Well, it's a translator and it translates text using various apis. :-)

Syntax: \<sourcelang\>-\<targetlang\> \<searchterm\>

A list of all language codes you can find [here](https://en.wikipedia.org/wiki/List_of_ISO_639-1_codes). ISO 639-1 as well as ISO 639-3 should work!


**Example:** en-de house

this will translate "house" into german (de)

**Example:** fra-eng maison

this will translate the french word "maison" into english

Simple, isn't it? :-)

Thanks for your feedback and kudos!
