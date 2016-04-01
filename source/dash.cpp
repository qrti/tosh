// dash.cpp 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "framebuffer.h"
#include "draw.h"
#include "dash.h"

// long press + repeat time in ms
#define LOP_TIME    1000
#define REP_TIME    500

// codes for intern commands must not interfere with external chars for example 0x09 TAB
#define CMD_INT_SHOWHIDE    0x01             
#define CMD_INT_QUIT        0x02   

// must be set if no other cmd
#define CMD_NOC         0x01
// long press single
#define CMD_LPS         0x02
// long press repeat
#define CMD_LPR         0x04
// short press hidden active
#define CMD_SPA         0x08
// long press hidden active
#define CMD_LPA         0x10                                                                          
                                                                                                                                                          
Dash::Dash(Console* con, Framebuffer* frb)
{    
    this->con = con;
    this->frb = frb;
    char* fbp = frb->getTargetfbp();
    int lineLen = frb->getLineLen();
    Draw::init(fbp, lineLen);
}

void Dash::initTiles(uint16_t xres, uint16_t yres)
{    
    this->xres = xres;
    this->yres = yres;    
    
    sidex = xres / tilesx;
    sidey = yres / tilesy;
    
    Tile* t = tile;
    
    for(uint16_t y=0; y<tilesy; y++){
        for(uint16_t x=0; x<tilesx; x++){
            t->left = x * sidex;
            t->top = y * sidey;
            t->width = sidex;
            t++->height = sidey;  
        }        
    }
    
    refresh();
}

void Dash::refresh()
{
    if(visible){                        // dash visible
        con->setMode(KD_GRAPHICS);
        setColor(2);                    // tile line color
        
        for(uint16_t x=sidex; x<xres; x+=sidex)
            Draw::line(x, 0, x, yres-1);
        
        for(uint16_t y=sidey; y<yres; y+=sidey)
            Draw::line(0, y, xres-1, y);    
            
        uint16_t s = sidex < sidey ? sidex : sidey;
            
        for(uint16_t x=0; x<tilesx; x++)
            for(uint16_t y=0; y<tilesy; y++)
                drawTile(tile+y*tilesx+x, s);
                
        frb->copy();        
        con->setMode(KD_TEXT);
    }
    
    nRefresh = false;
}       
    
void Dash::drawTile(Tile* t, uint16_t s)
{
    char* p = t->poly;
    
    if(t->status)
        highTile(t, t->status);
    
    if(p){
        uint16_t len;
        uint16_t left = t->left + (sidex - s) / 2;
        uint16_t top = t->top + (sidey - s) / 2;
        
        while((len = strlen(p) / 2)){           // len / 2 ignores color byte                
            setColor(*p++);                         
            uint16_t polyx[len], polyy[len];                     

            for(uint16_t i=0; i<len; i++){
                polyx[i] = left + s * *p++ / 100;
                polyy[i] = top + s * *p++ / 100;
            }
            
            Draw::fillPoly(polyx, polyy, len);
            p++;
        }
    }        
}    

#define GAP 4

void Dash::highTile(Tile* t, uint16_t status)
{
    setColor(status==1 ? 0 : 1);                // normal and long press highlight
    uint16_t polyx[4], polyy[4];                     

    polyx[0] = t->left + GAP;
    polyy[0] = t->top + GAP;
    polyx[1] = t->left + t->width - GAP;
    polyy[1] = t->top + GAP;    
    polyx[2] = t->left + t->width - GAP;
    polyy[2] = t->top + t->height - GAP;    
    polyx[3] = t->left + GAP;
    polyy[3] = t->top + t->height - GAP;       
    
    Draw::fillPoly(polyx, polyy, 4);
}

void Dash::touch(uint16_t tox, uint16_t toy, uint16_t tos)
{
   Tile* t = tile;

    for(uint16_t y=0; y<tilesy; y++){
        for(uint16_t x=0; x<tilesx; x++){
            if(t->left <= tox && t->left + t->width > tox && t->top <= toy && t->top + t->height > toy){
                if(tos == 1){                           // touch start
                    t->status = 1;
                    t->time = nowPlus(LOP_TIME);
                }
                else if(tos == 2){                      // touch release
                    if(t->status != 3)                  // if not repeat
                        execute(t);              
                         
                    t->status = 0;                      // done
                }
                
                nRefresh = true;                        // schedules a refresh 
                break;
            } 
            
            t++;
        }        
    }    
}

bool Dash::needRefresh()
{    
    if(nRefresh)                                        // no check if flag already set
        return true;                                    // display refresh
    
    int now = nowMillis();
    Tile* t = tile;
    
    for(uint16_t y=0; y<tilesy; y++){  
        for(uint16_t x=0; x<tilesx; x++){
            if(t->status==1 && t->time<now){            // long press
                t->status = *t->cmd & CMD_LPR ? 3 : 2;  // repeat / highlight
                return true;                            // display refresh
            }
            else if(t->status==3 && t->time<now){       // repeat
                execute(t);                             // execute
                return false;                           // no display refresh
            }
            
            t++;
        }
    } 

    return false;                                       // no display refresh
}    
    
void Dash::setColor(uint8_t c)
{
    Draw::setColor(color + c*4);
}
    
int Dash::nowPlus(int ms)
{
    return nowMillis() + ms;
}
    
int Dash::nowMillis()
{
    timeb tb;
    ftime(&tb);
    
    return tb.millitm + (tb.time & 0xfffff) * 1000;
}    

void Dash::execute(Tile* t)
{
    char* p = t->cmd;                           // get command string pointer
                                                
    if(p){                                      // if command string
        char c = *p++;                          // get command byte
                                                
        if(visible)                             // dash visible
            c |= CMD_SPA | CMD_LPA;             // execution possible
        else if(!c&CMD_SPA && !c&CMD_LPA)       // dash hidden and no active flags
            return;                             // no execution
                                                
        if(t->status==1 && c&CMD_SPA){          // short press 
            sendCmd(p);                         // send command
        }                                       
        else if(t->status==2 && c&CMD_LPA){     // long press
            if(c & CMD_LPS){                    // single execution
                p += strlen(p) + 1;             // set pointer to long press string
                sendCmd(p);                     // send command
            }                                   
        }                                       
        else if(t->status==3 && c&CMD_LPA){     // repeated execution
            p += strlen(p) + 1;                 // set pointer to long press string
            sendCmd(p);                         // send command
            t->time = nowPlus(REP_TIME);        // schedule next execution
        }
    }
}

void Dash::hideShow()
{
    if(visible){
        visible = false;
        frb->con2fbmap(tty, 1);                 // ttyx -> tft framebuffer
    }                                           
    else{                                       
        visible = true;                         
        frb->con2fbmap(tty, 0);                 // ttyx -> hdmi framebuffer, also redraws framebuffer        
    }                                           
}                                               
                                                
void Dash::sendCmd(char* p)                     
{                                               
    while(*p){                                  // read until termination
        if(*p == CMD_INT_SHOWHIDE)              // hide/show dash
            hideShow();                         
        else if(*p == CMD_INT_QUIT)             // exits main loop
            quit = true;                        
        else                                    
            ioctl(STDIN_FILENO, TIOCSTI, p);     
                                                
        p++;                                    // next char
    }
}
