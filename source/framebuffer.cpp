// framebuffer.cpp
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "framebuffer.h"

#define FB_DEVICE0      "/dev/fb0"
#define FB_DEVICE1      "/dev/fb1"

Framebuffer::Framebuffer()
{
}

Framebuffer::~Framebuffer()
{
    if(fb0 > 0){ 
        if(ioctl(fb0, FBIOPUT_VSCREENINFO, &orig_var0))
            printf("error: restoring variable information 0\n");    
            
        close(fb0);     
    } 
    
    if(fb1 > 0) close(fb1);
    if((int)fbp0 > 0) munmap(fbp0, screensize);
    if((int)fbp1 > 0) munmap(fbp1, screensize);    
}

int Framebuffer::init(int* xres, int* yres)
{    
    struct fb_fix_screeninfo fix0;
    struct fb_var_screeninfo var0;
    struct fb_fix_screeninfo fix1;
    struct fb_var_screeninfo var1;    
    
    if((fb0 = open(FB_DEVICE0, O_RDWR)) < 0){
        printf("error: could not open %s\n", FB_DEVICE0);
        return 1;
    }

    if(ioctl(fb0, FBIOGET_FSCREENINFO, &fix0) < 0){
        printf("error: ioctl FBIOGET_FSCREENINFO 0\n");
        return 1;
    }

    if(ioctl(fb0, FBIOGET_VSCREENINFO, &var0) < 0){
        printf("error: ioctl FBIOGET_VSCREENINFO 0\n");     
        return 1;
    }

    // - - - - - - - - - - - - - - - - -

    fb1 = open(FB_DEVICE1, O_RDWR);
    
    if(fb1 == -1){
        printf("error: could not open %s\n", FB_DEVICE1);
        return 1;
    }

    if(ioctl(fb1, FBIOGET_FSCREENINFO, &fix1) < 0){
        printf("error: ioctl FBIOGET_FSCREENINFO 1\n");
        return 1;
    }

    if(ioctl(fb1, FBIOGET_VSCREENINFO, &var1) < 0){
        printf("error: ioctl FBIOGET_VSCREENINFO 1\n");   
        return 1;
    }    
    
    // - - - - - - - - - - - - - - - - -

    memcpy(&orig_var0, &var0, sizeof(struct fb_var_screeninfo));

    var0.bits_per_pixel = var1.bits_per_pixel;
    var0.xres = var1.xres;
    var0.yres = var1.yres;
    var0.xres_virtual = var1.xres;
    var0.yres_virtual = var1.yres;
    
//    printf("%d %d %d %d %d\n", var0.xres, var0.yres, var0.xres_virtual, var0.yres_virtual, var0.bits_per_pixel);
//    printf("%d %d %d %d %d\n", var1.xres, var1.yres, var1.xres_virtual, var1.yres_virtual, var1.bits_per_pixel);
    
    if(ioctl(fb0, FBIOPUT_VSCREENINFO, &var0)){
        printf("error: setting variable information 0\n");
        return 1;
    }

    lineLen = fix1.line_length;
    screensize = fix1.smem_len; 
    
    fbp0 = (char*)mmap(0, screensize, PROT_READ|PROT_WRITE, MAP_SHARED, fb0, 0);
                    
    if((int)fbp0 == -1){
        printf("error: failed to mmap 0\n");
        return 1;
    }

    fbp1 = (char*)mmap(0, screensize, PROT_READ|PROT_WRITE, MAP_SHARED, fb1, 0);
                    
    if((int)fbp1 == -1){
        printf("error: failed to mmap 1\n");
        return 1;
    }

    *xres = var1.xres;
    *yres = var1.yres;

    if(*xres==0 || *yres==0){
        printf("error: invalid screen size\n");
        return 1;
    }    
    
    return 0;
}

char* Framebuffer::getTargetfbp()
{
    return fbp0;
}

int Framebuffer::getLineLen()
{
    return lineLen;
}

int Framebuffer::con2fbmap(int con, int fb)
{
    struct fb_con2fbmap map;
    
    map.console = con;                   
    map.framebuffer = fb;
 
    if(ioctl(fb0, FBIOPUT_CON2FBMAP, &map)){            // any fb handle is possible
        printf("error: ioctl FBIOPUT_CON2FBMAPs\n");
        return 1;
    }        
    
    return 0;
}

void Framebuffer::copy()
{
    memcpy(fbp1, fbp0, screensize);
}
    