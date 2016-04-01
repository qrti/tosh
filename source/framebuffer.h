// framebuffer.h
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_FRAMEBUFFER_h
#define SHELT_FRAMEBUFFER_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

class Framebuffer
{
public:
    Framebuffer();
   ~Framebuffer();
    int init(int*, int*);
    char* getTargetfbp();
    int getLineLen();    
    int con2fbmap(int, int);
    void copy();

private:    
    struct fb_var_screeninfo orig_var0;
    int fb0=0, fb1=0;
    char *fbp0=0, *fbp1=0;
    int lineLen;
    long int screensize;
};

#endif
