#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>


int main(int argc, char **argv) {
	pid_t pid = getppid();

	sleep(2);
	kill(pid, SIGINT);
	sleep(2);
	kill(pid, SIGILL);
	sleep(2);
	kill(pid, SIGQUIT);
	sleep(2);
	kill(pid, SIGTERM);
	sleep(2);

	exit(0);
}
