// process.h 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#ifndef SHELT_PROCESS_h
#define SHELT_PROCESS_h

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <unistd.h>
#include <dirent.h>

#define MAXARGS     8

class Process
{
public:
    Process();
    ~Process();
    
    bool checkSelf();
    bool launch(char*, char*);
  
private:    
    pid_t find(const char*);
    int countInst(const char*);    
    bool execute(char*, char*);
};

#endif