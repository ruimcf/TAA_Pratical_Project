CC=gcc
CFLAGS=-std=c99 -g

all: example

example: main.o dcel.o linklist.o
	$(CC) main.o dcel.o linklist.o -o example -I. -lglpk $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -I. $(CFLAGS)

dcel.o: dcel.c
	$(CC) -c dcel.c -I. $(CFLAGS)

linklist.o: linklist.c
	$(CC) -c linklist.c -I. $(CFLAGS)

clean:
	rm *.o example
