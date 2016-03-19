# Tosh - Touch on Shell

##`>>> under construction <<<`

#### **Adds touch screen abilities on shell level for Raspberry Pi**

Overlay dashboard for console applications, low cpu load, highly configurable ...

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen01.png)

Example configured as mp3 player, featuring [> cmus <](https://cmus.github.io/)

[Video Demo](http://) on YouTube

- - -

**preperation**
- for proper configured LCD and touch visit  
  [LCD and Touch Config](#lcd-and-touch-config)
- optionally  
  [Install cmus](#install-cmus)

**installation**

- download repository from github
- unzip and copy to Raspberry Pi  
  for example to: ~/tosh
- compile with  
  `$ make`
- run with  
  `$ sudo ./tosh`

**expansion**

- to run without sudo  
  [Run without sudo](#run-without-sudo)
- for own dashboard configurations  
  [Dashboard Configuration](#dashboard-configuration)

**tested**

 - Raspbian Jessie *Lite*
 - Raspberry Pi 2 B
 - Raspberry Pi B+
 - SainSmart LCD 480 x 320
 - WaveShare LCD 480 x 320

**not tested**

- Raspbian Jessie *Full* Desktop  
  X-Windows may catch touch events

- - -

#### Run without sudo

because Tosh reads from tty and vcsa devices it needs su rights

(todo)

- - -

#### Dashboard Configuration

dashboard configuration is done in [config.ini](https://github.com/qrti/tosh/blob/master/source/comfig.ini), grid size, tiles, shapes, colors, commands, start app, command line and some flags can be set, instructions are given in the comments

**symbols and functions** (used in example)

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen02.png)

|function    |function     |function    |function     |key press      |
|------------|-------------|------------|-------------|---------------|
|library<br>-|copy<br>-    |file<br>-   |hide<br>quit |*short<br>long*|
|window<br>- |remove<br>-  |-<br>-      |-<br>-       |*short<br>long*|
|expand<br>- |prev<br>back+|next<br>fwd+|up<br>up+    |*short<br>long*|
|stop<br>-   |pause<br>-   |play<br>-   |down<br>down+|*short<br>long*|
\+ repeat

**functions and keys**

|function                 |key   |
|-------------------------|------|
|library view             |1     |
|copy from file to library|a     |
|quit                     |qy    |
|file view                |5     |
|window change            |tab   |
|remove from library      |D     |
|expand library entry     |space |
|previous track           |z     |
|back seek                |h     |
|next track               |b     |
|forward seek             |l     |
|up entry                 |k     |
|down entry               |j     |
|stop                     |v     |
|pause                    |c     |
|play / choose entry      |return|

complete list of [cmus functions and keys](https://github.com/cmus/cmus/blob/master/Doc/cmus.txt)

- - -

#### Install cmus

install cmus without recommends to prevent some dnet-common and libdnet trouble  
`$ sudo aptitude -R install cmus`

if you need ffmpeg  
`$ sudo aptitude install cmus-plugin-ffmpeg`

or

install cmus comletely  
`$ sudo apt-get install cmus`

if your run into trouble with dnet-common and libdnet  
`$ sudo apt-get purge libdnet dnet-common`

to auto resume cmus  
`$ nano ~/.cmus/rc`
```
set resume=true
```

to auto login your Raspberry Pi  
`$ sudo raspi-config`
```
Boot Options / Console Autologin
```

autostart after login  
`$ nano ~/.bashrc`
```
cd ~/tosh  
sudo ./tosh
```

- - -

#### LCD and Touch Config

if your LCD does not run on Raspbian *Lite* yet, follow the steps given at  
[LCD and Touch Lite](#lcd-and-touch-lite)

Raspian *Full* versions with X-Windows have not been tested with Tosh yet

also be sure that your touchscreen is set as input device, add the following line if necessary and reboot  
`$ sudo nano /boot/config.txt`
```
dtoverlay=ads7846,speed=500000,penirq=17
```
`$ sudo reboot`

to check your input devices enter  
`$ cat /proc/bus/input/devices`

- - -

#### LCD and Touch Lite

steps for installing LCD and touch on Raspian *Lite*

check for current updates  
`$ sudo apt-get update`  
`$ sudo apt-get upgrade`  
`$ sudo apt-get clean`  

in order to do a firmware update that covers LCD framebuffer support, add rpi update script, current Raspbian versions with built in framebuffer support may make this step unnecessary  
`$ sudo apt-get update`  
`$ sudo apt-get install git`  
`$ sudo wget https://raw.github.com/Hexxeh/rpi-update/master/rpi-update -O /usr/bin/rpi-update && sudo chmod +x /usr/bin/rpi-update`

now that we have the script, update the firmware and reboot  
`$ sudo rpi-update`  
`$ sudo reboot`  

be sure SPI is enabled  
`$ sudo raspi-config`  
*advanced options/spi - enabled yes*  
or  
`$ sudo nano /boot/config.txt`
```
dtparam=spi=on
```

the following settings are for SainSmart and WaveShare 480 x 320 displays, depending on your device values may differ

first check the modules.conf, it should look something like this  
`$ sudo nano /etc/modules-load.d/modules.conf`
```
snd-bcm2835  
i2c-bcm2708  
i2c-dev  
spi_bcm2835  
flexfb  
fbtft_device
```

check flexfb.conf for *SainSmart*  
`$ sudo nano /etc/modprobe.d/flexfb.conf`
```
options flexfb width=320 height=480 regwidth=16 init=-1,0xb0,0x0,-1,0x11,-2,250,-1,0x3A,0x55,-1,0xC2,0x44,-1,0xC5,0x00,0x00,0x00,0x00,-1,0xE0,0x0F,0x1F,0x1C,0x0C,0x0F,0x08,0x48,0x98,0x37,0x0A,0x13,0x04,0x11,0x0D,0x00,-1,0xE1,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0xE2,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0x36,0x28,-1,0x11,-1,0x29,-3  
options fbtft_device debug=3 rotate=90 name=flexfb speed=16000000 gpios=reset:25,dc:24  
options ads7846_device model=7846 cs=1 gpio_pendown=17 keep_vref_on=1 swap_xy=0 pressure_max=255 x_plate_ohms=60 x_min=200 x_max=3900 y_min=200 y_max=3900
```

and for *WaveShare*  
`$ sudo nano /etc/modprobe.d/flexfb.conf`  
```
options flexfb width=480 height=320 regwidth=16 init=-1,0xb0,0x0,-1,0x11,-2,250,-1,0x3A,0x55,-1,0xC2,0x44,-1,0xC5,0x00,0x00,0x00,0x00,-1,0xE0,0x0F,0x1F,0x1C,0x0C,0x0F,0x08,0x48,0x98,0x37,0x0A,0x13,0x04,0x11,0x0D,0x00,-1,0xE1,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0xE2,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0x36,0xe8,-1,0x11,-1,0x29,-3  
options fbtft_device debug=3 rotate=0 name=flexfb speed=16000000 gpios=reset:25,dc:24  
options ads7846_device model=7846 cs=1 gpio_pendown=17 keep_vref_on=1 swap_xy=1 pressure_max=255 x_plate_ohms=60 x_min=200 x_max=3900 y_min=200 y_max=3900
```

with these values the displays are in landscape mode, Raspberry Pi board power plug on top side

now check cmdline.txt  
`$ sudo nano /boot/cmdline.txt`
```
dwc_otg.lpm_enable=0 console=tty1 console=ttyAMA0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait fbcon=map:11 fbcon=font:ProFont6x11 fsck.repair=yes root wait
```

the output looks much better if you slim the console font  
`$ sudo dpkg-reconfigure console-setup`  
*UTF-8 / Guess optimal character set / Terminus / 8 x 14*  
or  
`$ sudo nano /etc/default/console-setup`
```
CODESET="guess"  
FONTFACE="Terminus"  
FONTSIZE="8x14"
```

restart console to see changes  
`$ sudo /etc/init.d/console-setup restart`

- - -

#### Copyright (C) 2016
[qrt@qland.de](mailto:qrt@qland.de)
