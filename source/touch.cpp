// touch.cpp
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "touch.h"

// touch bounce suppression in ms
#define TOUCHBOUNCE     100

// raw touch deviation
#define TOUCHDEVI        40

Touch::Touch()
{
}

Touch::~Touch()
{
    if(fdt > 0) close(fdt);
}

int Touch::init(int xres, int yres)
{
    int event = getEventNum();
    
    if(event < 0){
        printf("error: could not find touch event number\n");
        return 1;
    }
    
    char buf[32];
    sprintf(buf, "/dev/input/event%d", event);
    printf("touch %s\n", buf);                  

    if((fdt = open(buf, O_RDONLY|O_NONBLOCK)) < 0){
        printf("error: could not open touchscreen\n");
        return 1;
    }        
                
    this->xres = xres;
    this->yres = yres;    
                                
    scalex = (maxx - minx) * 100 / xres;
    scaley = (maxy - miny) * 100 / yres;            
                
//    printf("screen %d %d\n", xres, yres);   
//    printf("touch  %d %d %d %d\n", minx, maxx, miny, maxy);
//    printf("scale  %d %d\n", scalex, scaley);
      
    return 0;    
}

int Touch::getEventNum()
{
    int fd;
    
    if((fd = open("/proc/bus/input/devices", O_RDONLY)) < 0){       // cat /proc/bus/input/devices
        printf("error: could not open input devices\n");            // int len = lseek(fd, 0, SEEK_END) + 1;
        return -1;
    }
    
    char c;
    int i=0, crit=0, event=-1;
    char buf[256];

    while(read(fd, &c, 1)){
        if(c == 0x0a){
            buf[i] = '\0';
            
            if(strlen(buf) == 0)
                crit = 0;
            
            if(strstr(buf, "N: ") && strstr(buf, "Touchscreen")){
                if(++crit == 2)
                    break;
            }
            else if(strstr(buf, "H: Handlers=mouse")){
                event = atoi(strstr(buf, "event") + 5);
                
                if(++crit == 2)
                    break;
            }
            
            i = 0;
        }
        else if(c!=0x0d && i<255){
            buf[i++] = c;    
        }
    }

    close(fd);    
    
    return event;
}

uint16_t Touch::get(uint16_t* tox, uint16_t* toy)
{
    static int32_t x0=0, x1=0, y0=0, y1=0;                  // touch coordinates, 2 sets for plausibility
    static bool ignore=false;                               // bounce ignore flag
    static int32_t rTime=0;                                 // release time
                                                            
    struct input_event e;                                   // input event structure
    bool checkBreak = false;
    
    while(read(fdt, &e, sizeof(struct input_event)) > 0){   // read queue while not empty
        if(e.type==EV_ABS && e.value>0){                    // filter absolute coordinate event with non zero value
            checkBreak = false;                             // assume no check of break

            if(e.code == 0){                                // x coordinate was sent
                if(!x0){                                    // store first coord if no previous value
                    x0 = e.value;
                }
                else if(!x1){                               // store second coord if no previous value
                    x1 = e.value;          
                    checkBreak = y0 && y1;                  // first and second x coord is stored, if y coords are also ready try to break loop
                }
            }
            else if(e.code == 1){                           // same with y coords
                if(!y0){
                    y0 = e.value;
                }
                else if(!y1){
                    y1 = e.value;  
                    checkBreak = x0 && x1;                    
                }                
            }
            
            if(checkBreak){                                 // all coords are ready
                if(abs(x1-x0) > TOUCHDEVI) x0 = x1 = 0;     // if first and second coords deviate too much, collect another pair
                if(abs(y1-y0) > TOUCHDEVI) y0 = y1 = 0; 
                if(x0 && x1 && y0 && y1) break;             // no coords have been reset, break loop for immediate touch response
            }
        }
        else if(e.type==EV_KEY && e.code==BTN_TOUCH){       // filter touch 'key' events 
            if(e.value == 0){                               // touch release
                rTime = eventTime(&e);                      // store release time
                x0 = x1 = y0 = y1 = 0;                      // reset coords
                return ignore ? 0 : 2;                      // 'no change' if bounce ignore or 'touch release'
            }
            else if(e.value == 1){                              // touch start 
                ignore = eventTime(&e) < rTime + TOUCHBOUNCE;   // ignore touch if time since last release is too short
            }
        }        
    }     
    
    if(!checkBreak || ignore)                               // if loop not left with break or if bounce ignore
        return 0;                                           // 'no change'
    
    if(touchcon == 0){                                      // WaveShare
        x0 = (x0 - minx) * 100 / scalex;                    // normal x-axis, scale to get screen coordinates
        y0 = (maxy - y0) * 100 / scaley;                    // invert y-axis
    }                                                        
    else{                                                   // SainSmart
        x0 = (maxx - x0) * 100 / scalex;                    // invert x-axis
        y0 = (y0 - miny) * 100 / scaley;                    // normal y-axis        
    }                                    
                                                            
    if(x0 < 0) x0 = 0;                                      // check and correct range
    if(x0 > xres-1) x0 = xres - 1;                          
    if(y0 < 0) y0 = 0;                                      
    if(y0 > yres-1) y0 = yres - 1;                          
                                                            
    *tox = x0;                                              // pass coords
    *toy = y0;                                              
                                                            
    return 1;                                               // 'touch start'
}

int32_t Touch::eventTime(input_event* e)
{
    return e->time.tv_sec * 1000 + e->time.tv_usec / 1000;
}    
