#### LCD and Touch Config

steps for installing LCD and touch on Raspian *Jessie* with SainSmart, WaveShare, or similar displays

install current updates  
`$ sudo apt-get update`  
`$ sudo apt-get upgrade`  
`$ sudo apt-get clean`  

(for Raspian Jessie *Lite*)  
in order to do a firmware update that covers LCD framebuffer support, add rpi update script, current Raspbian versions with built in framebuffer support may make this step unnecessary  
`$ sudo apt-get update`  
`$ sudo apt-get install git`  
`$ sudo wget https://raw.github.com/Hexxeh/rpi-update/master/rpi-update -O /usr/bin/rpi-update && sudo chmod +x /usr/bin/rpi-update`

(for Raspian Jessie *Lite*)  
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

these next values will configure the display to landscape mode, Raspberry Pi board power plug on top side

check flexfb.conf  
`$ sudo nano /etc/modprobe.d/flexfb.conf`
```
options flexfb width=480 height=320 regwidth=16 init=-1,0xb0,0x0,-1,0x11,-2,250,-1,0x3A,0x55,-1,0xC2,0x44,-1,0xC5,0x00,0x00,0x00,0x00,-1,0xE0,0x0F,0x1F,0x1C,0x0C,0x0F,0x08,0x48,0x98,0x37,0x0A,0x13,0x04,0x11,0x0D,0x00,-1,0xE1,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0xE2,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0x36,0x28,-1,0x11,-1,0x29,-3

# for 180 degree lcd rotation use this line instead (waveshare display)
#options flexfb width=480 height=320 regwidth=16 init=-1,0xb0,0x0,-1,0x11,-2,250,-1,0x3A,0x55,-1,0xC2,0x44,-1,0xC5,0x00,0x00,0x00,0x00,-1,0xE0,0x0F,0x1F,0x1C,0x0C,0x0F,0x08,0x48,0x98,0x37,0x0A,0x13,0x04,0x11,0x0D,0x00,-1,0xE1,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0xE2,0x0F,0x32,0x2E,0x0B,0x0D,0x05,0x47,0x75,0x37,0x06,0x10,0x03,0x24,0x20,0x00,-1,0x36,0xe8,-1,0x11,-1,0x29,-3

options fbtft_device debug=3 rotate=0 name=flexfb speed=16000000 gpios=reset:25,dc:24

# seems to be ignored, equivalent settings now in /boot/config.txt
#options ads7846_device model=7846 cs=1 gpio_pendown=17 keep_vref_on=1 swap_xy=1 pressure_max=255 x_plate_ohms=60 x_min=200 x_max=3900 y_min=200 y_max=3900

```

now check cmdline.txt  
`$ sudo nano /boot/cmdline.txt`
```
dwc_otg.lpm_enable=0 console=tty1 console=ttyAMA0,115200 root=/dev/mmcblk0p2 rootfstype=ext4 elevator=deadline rootwait fbcon=map:11 fbcon=font:ProFont6x11 fsck.repair=yes fbtft_device.rotate=0 root wait
```

also be sure that your touchscreen is set as input device and swapxy is set correctly, edit the following line if necessary  
`$ sudo nano /boot/config.txt`
```
dtoverlay=ads7846,speed=500000,penirq=17,swapxy=1
```

to check your input devices enter  
`$ cat /proc/bus/input/devices`

after the last step reboot  
`$ sudo reboot`

- - -

so far LCD and touch should work on the command line now and Tosh should run properly, anyway sometimes it is necessary to swap and invert touch axis or set touch limits on some systems

- axis swap is done in /boot/config.txt  
  look some lines above
- axis inversion and touch limits are set in
  section [TOUCHCON] of tosh [config.ini](../source/config.ini)

for sake of completeness, LCD and touch for Raspian desktop systems need two more steps to make x windows work correct (not needed for Tosh)

- touch axis inversion
- touch calibration

find detailed descriptions at  
[http://www.circuitbasics.com/raspberry-pi-touchscreen-calibration-screen-rotation/](http://www.circuitbasics.com/raspberry-pi-touchscreen-calibration-screen-rotation/)

- - -

last but not least, output looks much better on your LCD if you slim the console font  
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

[back to readme](../README.md)
