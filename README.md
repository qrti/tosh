# Tosh - Touch on Shell

#### **Adds touch screen abilities on shell level for Raspberry Pi**

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen01.png)

Example configured as mp3 player, featuring [> cmus <](https://cmus.github.io/)

- - -

**Configurable**

 - grid
 - symbols
 - commands
 - colors with alpha
 - start app

- - -

**Preperation**
- [optionally install cmus to your RasPi](#### Install cmus)
- download from github
- copy to /home/tosh on RasPi
- compile with make


- - -

**Tested**

 - Raspbian Jessie Lite
 - Raspberry Pi 2 B
 - Raspberry Pi B+
 - SainSmart 480 x 320 LCD
 - WaveShare 480 x 320 LCD

**Not Tested**

- Raspbian Jessie full desktop


- - -


####[cmus](https://cmus.github.io/) configuration
see [source/config.ini](https://github.com/qrti/tosh/blob/master/source/comfig.ini)

**key press symbols and functions**

![tosh with cmus](https://github.com/qrti/tosh/blob/master/images/screen02.png)

|function    |function     |function    |function     |key press      |
|------------|-------------|------------|-------------|---------------|
|library<br>-|copy<br>-    |file<br>-   |hide<br>quit |*short<br>long*|
|window<br>- |remove<br>-  |-<br>-      |-<br>-       |*short<br>long*|
|expand<br>- |prev<br>back+|next<br>fwd+|up<br>up+    |*short<br>long*|
|stop<br>-   |pause<br>-   |play<br>-   |down<br>down+|*short<br>long*|
\+ repeat

**cmus functions and keys**

|function                 |key   |
|-------------------------|------|
|library view             |1     |
|copy from file to library|a     |
|quit                     |qy    |
|file view                |5     |
|window change            |TAB   |
|remove from library      |D     |
|expand library entry     |SPACE |
|previous track           |z     |
|back seek                |h     |
|next track               |b     |
|forward seek             |l     |
|up entry                 |k     |
|down entry               |j     |
|stop                     |v     |
|pause                    |c     |
|play                     |RETURN|

[complete list of cmus functions and keys](https://github.com/cmus/cmus/blob/master/Doc/cmus.txt)

- - -


#### Install cmus



[TOC]












