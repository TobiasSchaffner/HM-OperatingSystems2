CC = gcc
CFLAGS = -Wall -ansi -std=c99 -pedantic

all: timeout.c
	$(CC) $(CFLAGS) timeout.c -o timeout

run: timeout
	./timeout

clean:
	rm -f timeout
