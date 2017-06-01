#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>

pid_t worker_pid = 0;

int isNumber(char *number) {
	int i = 0;
	if (number[0] == '-')
		return 0;
	for (; number[i] != 0; i++) {
	if (!isdigit(number[i]))
		return 0;
	}
	return 1;
}

void sig_handler(int signum) {
	if ( signum == SIGINT ) printf("%d: How rude!\n", signum);
	else if ( signum == SIGILL ) printf("%d: You be illin!\n", signum);
	else if ( signum == SIGQUIT ) printf("%d: Too legit to quit!\n", signum);
	else if ( signum == SIGTERM ) printf("%d: I'll be back!\n", signum);
	else if ( signum == SIGCHLD ) {
		if (!waitpid(worker_pid, NULL, WNOHANG)) printf("%d: My child is growing up!\n", signum);
		else printf("First you forked my child(%d), and now this...\n", worker_pid);
	}
	else printf("Illegal signal\n");
}

void init_signals() {
	struct sigaction newHandler;
	sigset_t block;
	sigfillset(&block);

	newHandler.sa_handler = sig_handler;
	newHandler.sa_mask = block;

	if( sigaction(SIGINT, &newHandler, NULL) == -1 ||
	    sigaction(SIGILL, &newHandler, NULL) == -1 ||
	    sigaction(SIGQUIT, &newHandler, NULL) == -1 ||
	    sigaction(SIGTERM, &newHandler, NULL) == -1 ||
	    sigaction(SIGCLD, &newHandler, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}
}

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Please provide a process to execute!\n");
		return 1;
	}

	worker_pid = fork();

	if (worker_pid < 0) {
		printf("Creation of Thread failed!");
		return 1;
	}
	else if (worker_pid > 0) {
		init_signals();
		while(1) {
			sleep(10);
			printf("waiting...\n");
		}
	}
	else {
		execvp(argv[1], &argv[1]);
		printf("Execution of command %s failed!\n", argv[2]);
		exit(1);
	}

	return 0;
}
