// draw.cpp
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "draw.h" 

void Draw::init(char* fbp, int lineLen)
{
    this->fbp = fbp;
    this->lineLen = lineLen;
}

void Draw::getPixel_rgb565(uint16_t x, uint16_t y, uint16_t* r, uint16_t* g, uint16_t* b)
{
    uint32_t offset = x * 2 + y * lineLen;
    uint16_t c = *((uint16_t*)(fbp + offset));              // rrrr rggg gggb bbbb
    
    *r = (c & 0xf800) >> 8;                                 // 0000 0000 rrrr r000
    *g = (c & 0x07e0) >> 3;                                 // 0000 0000 gggg gg00
    *b = (c & 0x001f) << 3;                                 // 0000 0000 bbbb b000
}

void Draw::setPixel_rgb565(uint16_t x, uint16_t y, uint16_t r, uint16_t g, uint16_t b)
{
    uint32_t offset = x * 2 + y * lineLen;
    uint16_t c = ((r & 0xf8) << 8) + ((g & 0xfc) << 3) + (b >> 3);  
    *((uint16_t*)(fbp + offset)) = c;
}

void Draw::setPixel(uint16_t x, uint16_t y)
{    
    uint16_t r, g, b;
    
    getPixel_rgb565(x, y, &r, &g, &b);
    
    r = (alpha * colr + r * (255 - alpha)) >> 8;
    g = (alpha * colg + g * (255 - alpha)) >> 8;
    b = (alpha * colb + b * (255 - alpha)) >> 8;
    
    setPixel_rgb565(x, y, r, g, b);
}

void Draw::line(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
    int16_t ax = 1, ay = 1;
    int16_t dx = x1 - x0;
    int16_t dy = y1 - y0;
    
    if(dx < 0){
        dx = -dx;
        ax = -ax;
    }
    
    if(dy < 0){
        dy = -dy;
        ay = -ay;
    }    
    
    if(dx!=0 && dy!=0){
        if(dx >= dy){
            int16_t r = dx / 2;
            
            for(int16_t x=0; x<dx; x++){
                r += dy;
                
                if(r >= dx){
                    r -= dx;
                    setPixel(x0, y0);
                    x0 += ax;
                    y0 += ay;
                }
                else{
                    setPixel(x0, y0);
                    x0 += ax;
                }
            }
        }
        else{
            int16_t r = dy / 2;
            
            for(int16_t y=0; y<dy; y++){
                r += dx;
                
                if(r >= dy){
                    r -= dy;
                    setPixel(x0, y0);
                    x0 += ax;
                    y0 += ay;                
                }
                else{
                    setPixel(x0, y0);
                    y0 += ay;                
                }
            }            
        }
    }
    else if(dx!=0 && dy==0){
        for(int16_t x=0; x<dx; x++){
            setPixel(x0, y0);
            x0 += ax;
        }
    }
    else if(dx==0 && dy!=0){
        for(int16_t y=0; y<dy; y++){
            setPixel(x0, y0);
            y0 += ay;            
        }
    }    
}

void Draw::fillPoly(uint16_t* polyX, uint16_t* polyY, uint16_t num_corners)
{    
    uint16_t i, j, nodes, swap, nodeX[num_corners];
    uint16_t polyLeft = 0xffff, polyRight = 0;
    uint16_t polyTop = 0xffff, polyBot = 0;
    
    for(i=0; i<num_corners; i++){
        if(polyX[i] < polyLeft)  polyLeft = polyX[i];
        if(polyX[i] > polyRight)  polyRight = polyX[i];
        if(polyY[i] < polyTop)  polyTop = polyY[i];
        if(polyY[i] > polyBot)  polyBot = polyY[i];            
    }
    
    for(uint16_t pixelY=polyTop; pixelY<polyBot; pixelY++){         //  loop through rows of the image
        nodes = 0;                                                  //  build a list of nodes
        j = num_corners - 1; 
        
        for(i=0; i<num_corners; i++){
            if((polyY[i]<pixelY && polyY[j]>=pixelY) || (polyY[j]<pixelY && polyY[i]>=pixelY))
                nodeX[nodes++] = polyX[i] + (pixelY - polyY[i]) * (polyX[j] - polyX[i]) / (polyY[j] - polyY[i]); 
            
            j = i; 
        }
        
        i = 0;                                                      // sort nodes, via a bubble sort
        
        while(i < nodes-1){
            if(nodeX[i] > nodeX[i+1]){
                swap = nodeX[i]; 
                nodeX[i] = nodeX[i+1]; 
                nodeX[i+1] = swap; 
                if(i) i--; 
            }
            else{
                i++; 
            }
        }
        
        for(i=0; i<nodes; i+=2){                                    // fill pixels between node pairs
            if(nodeX[i] >= polyRight) 
                break;
            
            if(nodeX[i+1] > polyLeft){
                if(nodeX[i] < polyLeft) 
                    nodeX[i] = polyLeft;
                
                if(nodeX[i+1] > polyRight) 
                    nodeX[i+1] = polyRight;
                
                for(uint16_t pixelX=nodeX[i]; pixelX<nodeX[i+1]; pixelX++) 
                    setPixel(pixelX, pixelY); 
            }
        }
    }
}

void Draw::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    colr = r;
    colg = g;
    colb = b;
    alpha = a;          // 0..255 transparent..opaque
}

void Draw::setColor(uint8_t* color)
{
    colr = *color;
    colg = *(color + 1);
    colb = *(color + 2);
    alpha = *(color + 3);  
}
