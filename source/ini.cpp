// ini.cpp
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "ini.h"

#define SECTIONS            7

#define SECT_STARTAPP       0
#define SECT_STARTARG       1
#define SECT_TOUCHCON       2
#define SECT_COLOR          3
#define SECT_NUMTILES       4
#define SECT_TILEPOLY       5
#define SECT_TILECMD        6

char const* section[SECTIONS] = { "STARTAPP", "STARTARG", "TOUCHCON", "COLOR", "NUMTILES",  "TILEPOLY", "TILECMD" };
char data[MAXDATA + 1];         // +1 for termination if used as string
                        
char* startApp;                 // global declarations for dash.h and tosh.cpp
char* startArg;                 //
uint8_t* color;                 //
uint8_t tilesx, tilesy;         //
Tile* tile;                     //

uint8_t touchcon;               // global declarations for touch.h                 
int minx;                       //
int maxx;                       //
int miny;                       //
int maxy;                       //
                                
Ini::Ini()
{
}

Ini::~Ini()
{
    if(file) fclose(file);
    if(color) free(color);
    
    if(tile){
        Tile* t = tile;
        
        for(uint16_t i=0; i<tilesx*tilesy; i++){
            if(t->poly) free(t->poly);
            if(t->cmd) free(t->cmd);
            t++;
        }
    }
                
    if(startApp) free(startApp);            
    if(startArg) free(startArg);            
    if(tile) free(tile);
}

bool Ini::init(char const* name)
{
    if((file=fopen(name, "r")) == 0){
        printf("error: cannot open %s\n", name);
        return 1;
    }
    
    return 0;
}

bool Ini::read()
{
    char line[MAXLINE];
    uint16_t lc = 1;
    
    while(fgets(line, MAXLINE, file)){
        char* p = line;
        strip(p);
        p = trim(p);        
        
        if(strlen(p)){
            if(*p == '['){
                if(allocData())
                    return true;
                
                lsc = lc;
                
                if(readSection(p))
                    return true;
            }
            else{
                if(readValues(p))
                    return true;
            }
        }
        
        lc++;
    };    
    
    if(dc)
        allocData();
    
    return false;
}

bool Ini::readSection(char* p)
{
    char* v;
    
    if((v = strchr(++p, ']'))){ 
        *v = '\0';
    }
    else{
        printf("error: invalid ini section [%s <-\n", p);
        return true;
    }
    
    for(sect=0; sect<SECTIONS; sect++)
        if(strcmp(section[sect], p) == 0)
            break;
    
    if(sect == SECTIONS)
        printf("warning: unknown ini section: [%s]\n", p);
    
    return false;
}

bool Ini::readValues(char* p)
{
    char* v = strtok(p, ",");

    while(v){
        if(dc >= MAXDATA){                                  // if more data on next line
            printf("error: ini section too long\n");
            return true;            
        }         
        
        v = trim(v);                                        // strip leading and trailing spaces
        
        if(v[0] == '\''){                                   // character literal
            if(v[2] != '\''){
                printf("error: invalid ini char\n");
                return true;                
            }
            
            data[dc++] = v[1];
        }
        else if(v[0]=='0' && v[1]=='x'){                    // hexadecimal
            data[dc++] = strtol(v, NULL, 16);
        }
        else if(v[0] == '\"'){                              // string
            uint16_t len = strlen(v);
            
            if(v[len-1] != '\"'){            
                printf("error: invalid ini string\n");
                return true;                   
            }
            
            len -= 2;                                       // strip quotation marks
            
            if(len+dc > MAXDATA){                           
                printf("error: ini string too long\n");
                return true;
            }
            
            for(int i=0; i<len; i++) 
                data[dc++] = v[i+1];
        }
        else{                                               // digits
            data[dc++] = atoi(v);
        }
        
        v = strtok(NULL, ",");                              // get next token
    }
        
    return false;
}

bool Ini::allocData()
{
    if(dc >= 0){
        if(sect == SECT_STARTAPP){
            if(dc > 0){
                startApp = (char*)malloc(dc + 1);
                strncpy(startApp, data, dc);
                startApp[dc] = '\0';           
            }
        }
        if(sect == SECT_STARTARG){
            if(dc > 0){
                startArg = (char*)malloc(dc + 1);
                strncpy(startArg, data, dc);
                startArg[dc] = '\0';           
            }
        }        
        else if(sect == SECT_TOUCHCON){
            if(dc != 9){
                printf("error: invalid section [%s]\n", section[sect]);
                return true;                
            }
            
            touchcon = data[0];    
            minx = data[1] * 100 + data[2];
            maxx = data[3] * 100 + data[4];
            miny = data[5] * 100 + data[6];
            maxy = data[7] * 100 + data[8];         
        }
        else if(sect == SECT_COLOR){
            color = (uint8_t*)malloc(dc);
            memcpy(color, data, dc);
        }    
        else if(sect == SECT_NUMTILES){
            if(dc != 2){
                printf("error: invalid section [%s]\n", section[sect]);
                return true;
            }
            
            tilesx = data[0];
            tilesy = data[1];        
            uint16_t numTiles = tilesx * tilesy;
            
            if(numTiles > MAXTILES){
                printf("error: max %d tiles allowed in [%s]\n", MAXTILES, section[sect]);
                return true;            
            }
            
            tile = (Tile*)malloc(numTiles * sizeof(Tile));
        }
        else if(sect == SECT_TILEPOLY){
            if(!tile){
                printf("error: [NUMTILES] must be defined before [TILEPOLY]\n");
                return true;                    
            }
            
            if(dc < 10){                     // x, y tile + color + min 3 poly points x, y + 0
                printf("warning: ignored invalid section in line %d\n", lsc);
                dc = 0;
                return false;                    
            }
            
            uint8_t x = data[0];
            uint8_t y = data[1];
            
            if(x>=tilesx || y>=tilesy){
                printf("warning: wrong tile dimension, ignored section in line %d\n", lsc);
                dc = 0;            
                return false;    
            }
            
            char* p = (char*)malloc(dc - 2);
            memcpy(p, data + 2, dc - 2);
            (tile + y * tilesx + x)->poly = p;
        }
        else if(sect == SECT_TILECMD){ 
            if(!tile){
                printf("error: [NUMTILES] must be defined before [TILECMD]\n");
                return true;                    
            }            
            
            if(dc < 5){                     // x, y tile + cmd + key + 0
                printf("warning: ignored invalid section in line %d\n", lsc);
                dc = 0;
                return false;                    
            }
            
            uint8_t x = data[0];
            uint8_t y = data[1];
            
            if(x>=tilesx || y>=tilesy){
                printf("warning: wrong tile dimension, ignored section in line %d\n", lsc);
                dc = 0;            
                return false;    
            }
            
            char* p = (char*)malloc(dc - 2);
            memcpy(p, data + 2, dc - 2);
            (tile + y * tilesx + x)->cmd = p;            
        }
    }
    
    dc = 0;
    return false;
}

void Ini::strip(char* p)
{
    char* v = p; 
    
    while(*p != '\0'){                  // strip comments  
        if(*p=='/' && *(p+1)=='/'){
            *p = '\0';
            break;
        }
        
        p++;
    }
    
    while(*v != '\0'){                  // tabs to single spaces  
        if(*v=='\t' || *v=='\r' || *v=='\n')
            *v = ' ';    
        
        v++;    
    }        
}

char* Ini::trim(char* p)
{
    while(*p == ' ')                    // trim leading spaces
        p++;
    
    if(*p == '\0')             
        return p;
    
    char* e = p + strlen(p) - 1;        // trim trailing spaces CR and LF
    
    while(e>p && *e==' ')    
        e--;
    
    *(e + 1) = '\0';    
        
    return p;    
}
    