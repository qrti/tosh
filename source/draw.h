// draw.h 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_DRAW_h
#define SHELT_DRAW_h

#include <stdint.h>

class Draw
{
public:
    void init(char*, int);
    void line(int16_t, int16_t, int16_t, int16_t);
    void fillPoly(uint16_t*, uint16_t*, uint16_t);
    void setColor(uint8_t, uint8_t, uint8_t, uint8_t);
    void setColor(uint8_t*);

private:    
    int lineLen;
    char *fbp;
    uint8_t colr, colg, colb, alpha;
    
    void setPixel_rgb565(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
    void getPixel_rgb565(uint16_t, uint16_t, uint16_t*, uint16_t*, uint16_t*);
    void setPixel(uint16_t, uint16_t);
};

#endif
