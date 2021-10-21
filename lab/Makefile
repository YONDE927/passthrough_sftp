ARGS = -DCYGFUSE
INCLUDES = -I/usr/includes
LDFLAGS = -L/usr/libs
LIBS = -lssh -lfuse-3.2


all: myssh.o
	gcc myssh.c -o myssh -g -Wall $(ARGS) $(INCLUDES) $(LDFLAGS) $(LIBS)
