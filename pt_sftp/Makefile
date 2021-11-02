ARGS = -DCYGFUSE
INCLUDES = -I/usr/includes 
INCLUDES2 = -I/cygdrive/c/'Program Files (x86)'/WinFsp/inc/
LDFLAGS = -L/usr/libs
LIBS = -lssh -lfuse-3.2


all: passthrough-fuse3.c
	gcc passthrough-fuse3.c -o passthrough -g -Wall $(ARGS) $(INCLUDES) $(LDFLAGS) $(LIBS)

