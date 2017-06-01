CC = gcc
CFLAGS = -Wall -std=c99 -pedantic

all: signalfun sendsignals loop

signalfun: signalfun.c
	$(CC) $(CFLAGS) signalfun.c -o signalfun

loop: loop.c
	$(CC) $(CFLAGS) loop.c -o loop

sendsignals: sendsignals.c
	$(CC) $(CFLAGS) sendsignals.c -o sendsignals

run_loop: signalfun loop
	./signalfun /home/osboxes/Documents/assignement_7/exercise_1/loop

run_test: signalfun sendsignals
	./signalfun /home/osboxes/Documents/assignement_7/exercise_1/sendsignals

clean:
	rm -f signalfun loop sendsignals
