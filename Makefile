# [ASCII Invader]
# File: Makefile
# Description: Makefile
# Author: Hayato OKUMOTO
# Copyright: (C)Hayato OKUMOTO, Yuta KOBAYASHI. 2013

TARGET = invader
SRCS = invader.c game.c drawing.c network.c
OBJS = ${SRCS:.c=.o}

HEADERS = game.h invader.h drawing.h network.h

CC = gcc
CCFLAGS = -Wall
LD = gcc
LDFLAGS = 
LIBS = -lc -lncurses

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) -c $<

$(OBJS): $(HEADERS) Makefile

clean:
	$(RM) $(TARGET) $(OBJS) *.o *~
