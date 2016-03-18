# Tosh - Touch on Shell

##`>>> under construction <<<`

#### **Adds touch screen abilities on shell level for Raspberry Pi**

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen01.png)

Example configured as mp3 player, featuring [> cmus <](https://cmus.github.io/)

**highly configurable**

 - overlay grid
 - overlay symbols
 - colors and alpha
 - commands
 - start app

- - -

**preperation**

- check for proper [LCD and touch configuration](#lcd-and-touch-configuration)
- optionally [Install cmus](#install-cmus)

**installation**

- download repository from github
- unzip and copy to RasPi
  for example to: /home/tosh
- compile with `$make`
- run with`$sudo ./tosh`

**expansion**

- or [run without sudo](#run-without-sudo)
- for defining own styles see [Configuration](#configuration)

**tested**

 - Raspbian Jessie Lite
 - Raspberry Pi 2 B
 - Raspberry Pi B+
 - SainSmart 480 x 320 LCD
 - WaveShare 480 x 320 LCD

**not Tested**

- Raspbian Jessie full desktop

- - -

#### LCD and Touch Configuration

Be sure that your touchscreen is set as input device, if not present, add the following line and reboot
`$sudo nano /boot/config.txt`
dtoverlay=ads7846,speed=500000,penirq=17

To check your input devices enter
`$cat /proc/bus/input/devices`

- - -

#### Run without sudo

- - -

#### Configuration

Configuration is done in [config.ini](https://github.com/qrti/tosh/blob/master/source/comfig.ini). Grid size, tiles, shapes, colors, commands, start app, command line and some flags can be set. Instructions are given in the example file's comments.

**key press symbols and functions**
used for cmus

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen02.png)

|function    |function     |function    |function     |key press      |
|------------|-------------|------------|-------------|---------------|
|library<br>-|copy<br>-    |file<br>-   |hide<br>quit |*short<br>long*|
|window<br>- |remove<br>-  |-<br>-      |-<br>-       |*short<br>long*|
|expand<br>- |prev<br>back+|next<br>fwd+|up<br>up+    |*short<br>long*|
|stop<br>-   |pause<br>-   |play<br>-   |down<br>down+|*short<br>long*|
\+ repeat

**functions and keys**
used for cmus

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

Complete list of [cmus functions and keys](https://github.com/cmus/cmus/blob/master/Doc/cmus.txt)

- - -

#### Install cmus

- - -

#### Copyright (C) 2016
[qrt@qland.de](mailto:qrt@qland.de)
