// console.h 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_CONSOLE_h
#define SHELT_CONSOLE_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/kd.h>
#include <linux/vt.h>
#include <sys/ioctl.h>

class Console
{
public:
    Console();
   ~Console();
    int init();
    int modified();
    int setMode(int);
    bool active();

private:    
    char *cbuf0=0, *cbuf1=0;  
    int fdv=0;
    int len;  
    
    int getTTY();
};

#endif
