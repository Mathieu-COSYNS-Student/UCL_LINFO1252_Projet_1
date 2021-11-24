CFLAGS=-std=c11 -pedantic -Wvla -Wall -Werror -pthread

all:
	make readers_writers

readers_writers: readers_writers.c utils.o
	gcc $(CFLAGS) -o readers_writers readers_writers.c utils.o

utils.o: utils.c utils.h
	gcc $(CFLAGS) -c utils.c

clean:
	rm -rf *.o
	rm -f readers_writers
