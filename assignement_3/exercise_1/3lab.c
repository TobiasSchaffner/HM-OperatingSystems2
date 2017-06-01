#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

int main() {
	int ret;
	int input;
	while (1) {
		printf("How many numbers do you want to sort? ");
		ret = scanf("%d", &input);
		if (ret != 1) {
			printf("\n");
			return ret;
		}
		if (input == 0) {
			printf("\n");
			return 0;
		}
		int *numbers = (int *) malloc(sizeof(int)*input);

		int i;
		for (i=0; i<input; ++i) {
			printf("Enter number %d: ", i);
			ret = scanf("%d", &numbers[i]);
			if (ret != 1) {
				printf("\nUnexpected end of file!\n");
				free(numbers);
				return ret;
			}
		}
		qsort(numbers, input, sizeof(int), cmpfunc);
		printf("\nI sorted your numbers to: ");
		for (i=0; i<input; ++i) {
			printf("%d ", numbers[i]);
		}
		printf("\n\n");
		free(numbers);
	}
	return 0;
}
