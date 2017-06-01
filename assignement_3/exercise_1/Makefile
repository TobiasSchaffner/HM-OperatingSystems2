CC = gcc
CFLAGS = -Wall -ansi -std=c99 -pedantic -ggdb


3lab: 3lab.c
	$(CC) $(CFLAGS) 3lab.c -o 3lab

run: 3lab
	./3lab

debug: 3lab
	gdb 3lab

clean:
	rm -f 3lab
