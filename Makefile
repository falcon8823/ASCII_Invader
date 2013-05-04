#MYLIBDIR	= ../mylib
#MYLIB			= $(MYLIBDIR)/mylib.a
#CFLAGS		= -I$(MYLIBDIR)
TARGET = invader
SRCS = invader.c game.c drawing.c
OBJS = ${SRCS:.c=.o}

HEADERS = game.h invader.h drawing.h

CC = gcc
CCFLAGS = -Wall
LD = gcc
LDFLAGS = 
LIBS = -lc -lncursesw

$(TARGET): $(OBJS)
	$(LD) $(OBJS) $(LDFLAGS) -o $(TARGET) $(LIBS)

.c.o:
	$(CC) $(CCFLAGS) -c $<

$(OBJS): $(HEADERS) Makefile


clean:
	$(RM) $(TARGET) $(OBJS) *.o *~
