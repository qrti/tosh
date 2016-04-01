// console.cpp 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "console.h"

int tty;                                                // global declaration for dash.h and tosh.cpp  

Console::Console()
{
}

Console::~Console()
{
    if(fdv > 0) close(fdv);                             // close and free
    if(cbuf0) free(cbuf0);
    if(cbuf1) free(cbuf1);
}

int Console::init()
{
    if((tty = getTTY()) < 0)                            // get TTY device number
        return 1;
    
    char vcsa[16];                                      // assemble device path
    sprintf(vcsa, "/dev/vcsa%d", tty);
    
    if((fdv = open(vcsa, O_RDONLY)) < 0){               // open virtual screen device
        printf("error: could not open %s\n", vcsa);   
        return 1;
    }

    struct{                                             // screen info
        unsigned char lines, cols, x, y;
    }screen;
        
    read(fdv, &screen, 4);                              // get screen info
    len = 4 + screen.lines * screen.cols * 2;           // calc len
    
    cbuf0 = (char*)malloc(len);                         // allocate buffers
    cbuf1 = (char*)malloc(len);
    
    modified();                                         // init read
    return 0;                                           // no error
}

int Console::modified()
{
    lseek(fdv, 0, SEEK_SET);                            // seek to start
    read(fdv, cbuf0, len);                              // store console to buffer
    int mod = bcmp(cbuf0, cbuf1, len);                  // byte compare buffers
                                                        
    char* temp = cbuf0;                                 // swap buffers
    cbuf0 = cbuf1;                                      
    cbuf1 = temp;                                       
                                                        
    return mod;                                         // return modified
}

// mode KD_GRAPHICS or KD_TEXT
//
int Console::setMode(int mode)
{
    if(ioctl(STDIN_FILENO, KDSETMODE, mode) == -1){     // send command
	    printf("error: could not set mode for STDIN device\n");    
	    return 1;
	}
    
    return 0;
}

int Console::getTTY()
{
    char* name;
    
    if((name = ttyname(STDIN_FILENO)) == 0){
        printf("error: could not get TTY name\n");
        return -1;
    }
    
    char* p;
    
    if((p = strstr(name, "tty")) == 0){
        printf("error: program runs on TTY console only\n");
        return -1;        
    }
    
    int n = atoi(p + 3);

    if(n > 63){
        printf("error: invalid console TTY%d\n", n);
        return -1;        
    }

    return n;
}

bool Console::active()
{
    struct vt_stat vts;
    ioctl(STDIN_FILENO, VT_GETSTATE, &vts);
    return vts.v_active == tty;                         // origin console active or not
}


// #define TTY_DEVICE      "/dev/tty1"
// 
// int fdt=0;
// 
// if((fdt = open(TTY_DEVICE, O_RDWR)) < 0){                 // open console device
//     printf("error: could not open %s\n", TTY_DEVICE);   
//     return 1;        
// }   
// 
// if(fdt > 0) close(fdt);  