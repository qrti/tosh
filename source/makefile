# makefile
# part of tosh - touch on shell
#
# published under the terms of BSD 2-clause license
# Copyright (c) 2016, qrt@qland.de 
# All rights reserved

#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C define  CC = gcc
CC = g++
CFLAGS = -std=c++14

default: tosh

tosh: ini.o process.o console.o framebuffer.o touch.o draw.o dash.o tosh.o
	$(CC) $(CFLAGS) -o tosh ini.o process.o console.o framebuffer.o touch.o draw.o dash.o tosh.o

tosh.o: tosh.cpp
	$(CC) $(CFLAGS) -c tosh.cpp

dash.o: dash.cpp dash.h 
	$(CC) $(CFLAGS) -c dash.cpp

draw.o: draw.cpp draw.h 
	$(CC) $(CFLAGS) -c draw.cpp

touch.o: touch.cpp touch.h 
	$(CC) $(CFLAGS) -c touch.cpp

framebuffer.o: framebuffer.cpp framebuffer.h 
	$(CC) $(CFLAGS) -c framebuffer.cpp

console.o: console.cpp console.h 
	$(CC) $(CFLAGS) -c console.cpp

process.o: process.cpp process.h 
	$(CC) $(CFLAGS) -c process.cpp

ini.o: ini.cpp ini.h 
	$(CC) $(CFLAGS) -c ini.cpp


clean: 
	$(RM) tosh *.o *~