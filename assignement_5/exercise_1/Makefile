CC = gcc
CFLAGS = -Wall -ansi -std=gnu99 -pedantic -ggdb


myls: myls.c modeToStr.c
	$(CC) $(CFLAGS) myls.c modeToStr.c -o myls

run: myls
	./myls

debug: myls
	gdb myls

clean:
	rm -f myls
