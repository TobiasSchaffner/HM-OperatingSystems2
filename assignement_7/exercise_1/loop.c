#define _POSIX_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<unistd.h>


int main() {
	while(1) {
		sleep(1);
	}
	exit(0);
}
