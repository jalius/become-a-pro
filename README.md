this is a cheat for the 64 bit linux csgo (tested on ubuntu 14.04 and 16.04)


install additional dependencies:
```bash
apt-get install libboost-all-dev libx11-dev libx11-dev:i386 libxtst-dev libconfig++-dev build-essential cmake
```
install updated cmake:
```bash
cmake --version 
```
if your cmake is version 3 or higher you can skip this step:


```bash
sudo add-apt-repository ppa:george-edison55/cmake-3.x
```
if cmake is installed:
```bash
sudo apt-get upgrade cmake
```
if cmake is not installed: 
```bash
sudo apt-get install cmake
```

how to build:

install the dependencies (above).

navigate to the base directory (usually ~/become-a-pro).

run command:
```bashcmake .```

run command: 
```bashmake```

rename settings-example.cfg to settings.cfg and modify settings to your liking.

run become-a-pro as super user:

```bash sudo ./become-a-pro```


NOTE:
please make sure you have settings.cfg in the same directory as the terminal's cd, otherwise you will experience problems with libconfig.
