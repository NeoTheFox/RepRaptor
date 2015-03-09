![RepRaptor logo](http://reprap.org/mediawiki/images/b/b0/RepRaptor_logo2.png)
# RepRaptor
A Qt RepRap gcode sender/host controller aimed to be fast and minimalistic.

Right now the project is in early stage. This means some features are still absent, but it is already usable.

## Screenshots
![Screenshot](http://reprap.org/mediawiki/images/6/6c/RepRaptor-v0.2.png)

## Install via PPA
Since 0.3 release PPA is available for Ubuntu 14.04 and Linux Mint 17.
```
sudo add-apt-repository ppa:soniczerops/repraptor-dev
sudo apt-get update
sudo apt-get install repraptor
```

## Building on Ubuntu 14.04

```
sudo apt-get install libqt5serialport5-dev git build-essential 

git clone https://github.com/NeoTheFox/RepRaptor && cd RepRaptor

qmake RepRaptor.pro

make
```
## Links
- [Binary release downloads](https://github.com/NeoTheFox/RepRaptor/releases)
- [RepRap wiki](http://reprap.org/wiki/RepRaptor)
- [Windows dev builds](http://reprap.org/wiki/File:RepRaptor-000-windows.zip)

[![Support via Gratipay](https://cdn.rawgit.com/gratipay/gratipay-badge/2.3.0/dist/gratipay.svg)](https://gratipay.com/NeoTheFox/)

