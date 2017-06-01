#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<sys/types.h>
#include<signal.h>

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
int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Please provide a timeout and a command to execute!\n");
		return 1;
	}
	if (!isNumber(argv[1])){
		printf("Please provide a positive Number for the timeout!!\n");
		return 1;
	}

	int status;
	pid_t worker_pid = fork();

	if (worker_pid < 0) {
		printf("Creation of Thread failed!");
		return 1;
	}
	else if (worker_pid > 0) {

		pid_t timeout_pid = fork();

		if (timeout_pid < 0) {
			printf("Creation of Thread failed!");
			return 1;
		}
		else if (timeout_pid > 0) {
			// parent
			int pid = wait(&status);
			if (pid == worker_pid) pid = timeout_pid;
			else pid = worker_pid;
			kill(pid, SIGKILL);
		}
		else {
			// timeout child
			sleep(strtol(argv[1], NULL, 10));
			return 0;
		}
	}
	else {
		// worker child
		execvp(argv[2], &argv[2]);
		printf("Execution of command %s failed!\n", argv[2]);
		exit(1);
	}

	return 0;
}
