// tosh.cpp V0.91 - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

// V0.9     initial version
// V0.91    corrections, fixes

#include "ini.h"
#include "process.h"
#include "console.h"
#include "framebuffer.h"
#include "touch.h"
#include "draw.h"
#include "dash.h"

extern char* startApp;          // defined in ini.cpp
extern char* startArg;          //
extern int tty;                 //            console.cpp

bool quit=false;                // global declaration for dash.h

int main(int argc, char* argv[])
{
    printf("tosh V0.91 160330 (c) qrt@qland.de\n");
    sleep(2);
    
    Ini config;
    
    if(config.init("config.ini"))
        return 1;
    
    if(config.read())
        return 1;

    // - - - - - - - - - - - - - - - - -
    
    Console console;
    
    if(console.init())
        return 1;    
            
    // - - - - - - - - - - - - - - - - -

    Process process;

    if(process.checkSelf())
        return 1;

    if(startApp)
        process.launch(startApp, startArg);

    // - - - - - - - - - - - - - - - - -
    
    Framebuffer framebuffer;
    int xres, yres;
    
    if(framebuffer.init(&xres, &yres))
        return 1;        
        
    // - - - - - - - - - - - - - - - - -
        
    Touch touch;
    
    if(touch.init(xres, yres))
        return 1;      

    // - - - - - - - - - - - - - - - - -
    
    Dash dash(&console, &framebuffer);
    dash.initTiles(xres, yres);
    
    if(framebuffer.con2fbmap(tty, 0))               // ttyx -> hdmi framebuffer
        return 1;                                   
                                                    
    dash.refresh();                                 // initially draw dash
                                                    
    uint16_t tox, toy, tos;                         // touch x, y, status
    bool conin=false;                               // console inactive = false
                                                    
    while(!quit){                                   
        tos = touch.get(&tox, &toy);                // eat touch events even if console inactive
        
        if(console.active()){                       // if console is active
            if(tos)                                 // if touch events
                dash.touch(tox, toy, tos);          // handle them
            
            if(dash.needRefresh() || console.modified() || conin){      // if refresh needed
                dash.refresh();                                         // refresh dash
                conin = false;                                          // reset console incative flag
            }
        }
        else{                                       // if console is inactive
            conin = true;                           // set flag
        }
          
        usleep(100000);                             // sleep 100 ms
    }                                               
                                                    
    if(framebuffer.con2fbmap(tty, 1))               // ttyx -> tft framebuffer
        return 1;    

    printf("%s quit\n", argv[0]);
    return 0;
}

// -----------------------------------------------------------------------------

//#include <iostream>
//std::cout << ".";

// "       list    > cat /proc/bus/input/devices\n"
//               "       edit    > /boot/config.txt\n"
//               "       and add > dtoverlay=ads7846,speed=500000,penirq=17\n"