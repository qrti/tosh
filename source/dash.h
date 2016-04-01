// dash.h 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_DASH_h
#define SHELT_DASH_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <linux/kd.h>
#include <sys/timeb.h>

#include "ini.h"
#include "console.h"

extern uint8_t* color;              // defined in ini.cpp
extern uint8_t tilesx, tilesy;      //
extern Tile* tile;                  //
extern int tty;                     //            console.cpp    
extern bool quit;                   //            tosh.cpp

class Dash : public Draw
{
public:
    Dash(Console*, Framebuffer*);
    void initTiles(uint16_t, uint16_t);
    void touch(uint16_t, uint16_t, uint16_t);
    bool needRefresh();
    void refresh();
            
private:    
    uint16_t xres, yres;
    uint16_t sidex, sidey;
    Console* con;
    Framebuffer* frb;
    bool nRefresh;
    bool visible = true;     
    
    void drawTile(Tile*, uint16_t);
    void setColor(uint8_t);
    void highTile(Tile*, uint16_t);
    int nowPlus(int);
    int nowMillis();
    void execute(Tile*);
    void hideShow();
    void sendCmd(char*);
};

#endif
