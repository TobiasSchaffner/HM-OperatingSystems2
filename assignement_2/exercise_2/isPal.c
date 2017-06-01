#include <string.h>
#include <stdio.h>

int isPal(char *input) {
	unsigned long length = strlen(input);
	if (length == 0) return -1;

	char inverse[length + 1];

	int input_pos;
	for(input_pos = 0; input_pos < length; ++input_pos) {
		inverse[input_pos] = input[(length - 1) - input_pos];
	}

	inverse[length] = '\0';
	return strcmp(input, inverse);
}
