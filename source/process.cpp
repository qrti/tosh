// process.cpp 
// part of tosh - touch on shell
//
// published under the terms of BSD 2-clause license
// Copyright (c) 2016, qrt@qland.de 
// All rights reserved

#include "process.h"

extern char* __progname;        // argv[0] may have ./ in front

Process::Process()
{
}

Process::~Process()
{
}

bool Process::checkSelf()
{
    if(countInst(__progname) > 1){
        printf("error: %s already running\n", __progname); 
        return true;
    }
    
    return false;
}

bool Process::launch(char* name, char* arg)
{
    bool error = true;
    
    if(find(name) == -1){
        if(!execute(name, arg)){
            printf("launched %s\n", name); 
            error = false;
        }
        else{    
            printf("warning: could not launch %s\n", name); 
        }
    }
    else{
        printf("warning: %s already running\n", name); 
    }    
    
    return error;
}

pid_t Process::find(const char* name) 
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long pid;
    char pname[100] = { 0, };
    char state;
    FILE *fp=NULL; 

    if(!(dir = opendir("/proc"))){
        perror("warning: cannot open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL){
        long lpid = atol(ent->d_name);
        
        if(lpid < 0)
            continue;
            
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if(fp){
            if((fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3){
                printf("warning: process fscanf failed\n");
                fclose(fp);
                closedir(dir);
                return -1; 
            }
            
            if(!strcmp(pname, name)){
                fclose(fp);
                closedir(dir);
                return (pid_t)lpid;
            }
            
            fclose(fp);
        }
    }

    closedir(dir);
    return -1;
}

int Process::countInst(const char* name)
{
    DIR* dir;
    struct dirent* ent;
    char buf[512];

    long pid;
    char pname[100] = { 0, };
    char state;
    FILE *fp=NULL; 

    int num=0;

    if(!(dir = opendir("/proc"))){
        perror("warning: cannot open /proc");
        return -1;
    }

    while((ent = readdir(dir)) != NULL){
        long lpid = atol(ent->d_name);
        
        if(lpid < 0)
            continue;
            
        snprintf(buf, sizeof(buf), "/proc/%ld/stat", lpid);
        fp = fopen(buf, "r");

        if(fp){
            if((fscanf(fp, "%ld (%[^)]) %c", &pid, pname, &state)) != 3){
                printf("warning: process fscanf failed\n");
                fclose(fp);
                closedir(dir);
                return -1; 
            }
            
            if(!strcmp(pname, name))
                num++;
            
            fclose(fp);
        }
    }

    closedir(dir);
    return num;    
}

bool Process::execute(char* name, char* arg)
{
    pid_t pid = fork();                         // fork process

    if(pid == 0){                               // child
        char* args[MAXARGS+2];                  // args[0] + termination
        int i=0;
        
        args[i++] = name;                       // args[0]
        
        if(arg != NULL){                        // if arguments
            char* v = strtok(arg, " ");
            
            while(v && i<MAXARGS+1){            // seperate arguments
                args[i++] = v;
                v = strtok(NULL, " ");       
            }
        }
        
        args[i] = 0;                            // terminate arguments
        
        const char* p = getenv("SUDO_USER");    // sudo user
        if(p == NULL) p = getenv("USER");       // user
        if(p == NULL) p = "pi";                 // default
        
        int len = strlen(p);                    // user and home
        char user[len + 6];
        sprintf(user, "USER=%s", p);
        char home[len + 12];
        sprintf(home, "HOME=/home/%s", p);
        char* const env[] = { (char*)"TERM=linux", user, home, (char*)0 };
        
        execve(name, args, env);                // execute 
        exit(0);                                // exit
    }
    else if(pid < 0){                           // error
        return true;
    }
    else{                                       // parent
    }    
    
    return false;                               // no error
}