ARGS = -DCYGFUSE
INCLUDES = -I/usr/includes 
INCLUDES2 = -I/cygdrive/c/'Program Files (x86)'/WinFsp/inc/
LDFLAGS = -L/usr/libs
LIBS = -lssh -lfuse-3.2


all: myssh.o
	gcc myssh.c -o myssh -g -Wall $(ARGS) $(INCLUDES) $(LDFLAGS) $(LIBS)
