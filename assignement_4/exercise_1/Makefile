CC = gcc
CFLAGS = -Wall -ansi -std=c99 -pedantic -ggdb

ALL: swapFile swapFile2

swapFile: swapFile.c
	$(CC) $(CFLAGS) swapFile.c -o swapFile

run: swapFile
	./swapFile

debug: swapFile
	gdb swapFile

swapFile2: swapFile2.c
	$(CC) $(CFLAGS) swapFile2.c -o swapFile2

run2: swapFile2
	./swapFile2

debug2: swapFile2
	gdb swapFile2

clean:
	rm -f swapFile swapFile2
