CC = gcc
CFLAGS = -Wall -ansi -std=c99 -pedantic -ggdb


cat: cat.c
	$(CC) $(CFLAGS) cat.c -o cat

run: cat
	./cat

debug: cat
	gdb cat

clean:
	rm -f cat
