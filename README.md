this is a cheat for the 64 bit linux csgo (tested on ubuntu 14.04, 16.04, and 17.04)


install additional dependencies:
```bash
apt-get install libboost-all-dev libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential libqt4-dev qt4-qmake
```

how to build:

install the dependencies (above).

navigate to the base directory (usually ~/become-a-pro_esp).

run command:
```bash
qmake ./become-a-pro_esp.pro
```
run command: 
```bash
make
```
rename settings-example.cfg to settings.cfg and modify settings to your liking.

run become-a-pro_esp as super user:

```bash 
sudo ./become-a-pro_esp
```


NOTE:
please make sure you have settings.cfg in the same directory as the terminal's cd, otherwise you will experience problems with libconfig.
