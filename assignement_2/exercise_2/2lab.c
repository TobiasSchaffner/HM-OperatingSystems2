#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "isPal.h"

int main() {
	int ret;
	printf("**Lab2 Palindrome Checker**\n");
	printf("type \"quit\" to end the program\n");
	while (1) {
		char input[1024];
		printf("Enter a word: ");
		ret = scanf("%1024s", input);
		if (ret != 1) {
			return ret;
		}
		int ch;
		while ((ch = getchar()) != '\n' && ch != EOF) ;
		if (strcmp(input, "quit") == 0) {
			return 0;
		}
		ret = isPal(input);
		if (ret == 0) {
			printf("%s is a palindrome.\n", input);
		} else {
			printf("%s is not a palindrome.\n", input);
		}
	}
	return 0;
}
