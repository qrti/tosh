// ini.h
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_INI_h
#define SHELT_INI_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MAXLINE         256
#define MAXTILES        256
#define MAXDATA         256

typedef struct{
    uint16_t left, top;
    uint16_t width, height;
    char* poly; 
    char* cmd;         
    uint16_t status;
    int time;                
}Tile;

class Ini
{
public:
    FILE* file=0;
    
    Ini();
    ~Ini();
    bool init(char const*);
    bool read();
    
private:    
    uint16_t sect=0;
    int16_t dc=-1;
    int16_t lsc=0;
        
    bool readSection(char*);
    bool readValues(char*);   
    bool allocData();
    void strip(char*);
    char* trim(char*);
};

#endif
