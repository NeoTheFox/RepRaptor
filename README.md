![RepRaptor logo](http://reprap.org/mediawiki/images/b/b0/RepRaptor_logo2.png)
# RepRaptor [![Build Status](https://travis-ci.org/NeoTheFox/RepRaptor.svg?branch=master)](https://travis-ci.org/NeoTheFox/RepRaptor)
A Qt RepRap gcode sender/host controller aimed to be fast and minimalistic.

Right now the project is in early stage. This means some features are still absent, but it is already usable.

## Screenshots
![Screenshot](http://reprap.org/mediawiki/images/6/6c/RepRaptor-v0.2.png)

## Install via PPA
Since 0.3 release PPA is available for Ubuntu 14.04 and Linux Mint 17 (and later).
```
sudo add-apt-repository ppa:soniczerops/repraptor-dev
sudo apt-get update
sudo apt-get install repraptor
```

## Building on Ubuntu 14.04

```
sudo apt-get install qt5-default libqt5serialport libqt5serialport5-dev git build-essential libudev-dev

git clone https://github.com/NeoTheFox/RepRaptor && cd RepRaptor

qmake RepRaptor.pro

make
```
And if you want to install system-wide
```
sudo make install
```

## Links
- [Binary release downloads (Windows, Linux)](https://github.com/NeoTheFox/RepRaptor/releases)
- [RepRap wiki](http://reprap.org/wiki/RepRaptor)
- [AUR](https://aur.archlinux.org/packages/repraptor-git/)

[![Support via Gratipay](https://cdn.rawgit.com/gratipay/gratipay-badge/2.3.0/dist/gratipay.svg)](https://gratipay.com/NeoTheFox/)

