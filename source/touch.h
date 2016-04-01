// touch.h 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_TOUCH_h
#define SHELT_TOUCH_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/input.h>
#include <sys/timeb.h>

#include "ini.h"

extern uint8_t touchcon;            // definition in ini.cpp
extern int minx;  
extern int maxx; 
extern int miny; 
extern int maxy; 

class Touch
{
public:
    Touch();
   ~Touch();
    int init(int, int);
    uint16_t get(uint16_t*, uint16_t*);

private:    
    int fdt=0;
    int xres, yres;
    int scalex, scaley;             // scale * 100
        
    int getEventNum();
    int32_t eventTime(input_event*);
};

#endif
