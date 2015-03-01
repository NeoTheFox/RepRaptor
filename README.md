![RepRaptor logo](http://reprap.org/mediawiki/images/b/b0/RepRaptor_logo2.png)
# RepRaptor
A Qt RepRap gcode sender/host controller

Right now the project is in early stage, use only for development testing!

## Screenshots
![RepRaptor printing a file](http://reprap.org/mediawiki/images/2/2d/RepRaptor_screenshot0.png)

## Building on Ubuntu 14.04.2

```
sudo apt-get install libqt5serialport5-dev git build-essential 

git clone https://github.com/NeoTheFox/RepRaptor && cd RepRaptor

qmake RepRaptor.pro

make
```

