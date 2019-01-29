INCLUDE = -I/usr/X11R6/include/
LIBDIR  = -L/usr/X11R6/lib

COMPILERFLAGS = -Wall -g
CC = gcc
CFLAGS = $(COMPILERFLAGS) $(INCLUDE)
LIBRARIES = -lX11 -lglut -lGL -lGLU -lm -lpng
OBJECTS =  defs.o

All: opengl

opengl.o: readdata.c

opengl: opengl.o $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $(OBJECTS) $(LIBDIR) $< $(LIBRARIES)
