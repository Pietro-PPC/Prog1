CFLAGS = -ansi -Wall --std=gnu99
LDLIBS = -lncurses

all: space_invaders

space_invaders: lib_invaders.o lib_lista.o main.c
	gcc lib_invaders.o lib_lista.o main.c -o space_invaders $(CFLAGS) $(LDLIBS)

lib_invaders.o: lib_invaders.c lib_invaders.h
	gcc -c lib_invaders.c $(CFLAGS)

lib_lista.o: lib_lista.c lib_lista.h
	gcc -c lib_lista.c $(LDLIBS)

clean: 
	rm -rf *.o

purge: clean
	rm space_invaders