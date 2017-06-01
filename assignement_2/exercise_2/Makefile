CC = gcc
CFLAGS = -Wall -ansi -std=c99 -pedantic


2lab: isPal.o 2lab.o
	$(CC) $(CFLAGS) isPal.o 2lab.o -o 2lab

isPal.o: isPal.c
	$(CC) $(CFLAGS) -c isPal.c

2lab.o: 2lab.c isPal.h
	$(CC) $(CFLAGS) -c 2lab.c

run: 2lab
	./2lab

clean:
	rm -f *.o 2lab
