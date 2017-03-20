CC=gcc
CFLAGS=-std=c99 -g

all: example

example: main.o dcel.o
	$(CC) main.o dcel.o -o example -I. $(CFLAGS)

main.o: main.c
	$(CC) -c main.c -I. $(CFLAGS)

dcel.o: dcel.c
	$(CC) -c dcel.c -I. $(CFLAGS)

clean:
	rm *.o example
