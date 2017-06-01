#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>

int main(int argc, char *argv[]) {

	if (argc != 2) {
		printf("Please provide a filename!\n");
		return 1;
	}

	int ret, fdin;
	char c;

	fdin = open(argv[1], O_RDONLY);
	while ((ret = read(fdin, &c, 1)) > 0) {
		if (write(1, &c, ret) != ret) {
			perror("write");
			return 1;
		}
	}
	if (ret < 0) {
		perror("read");
		return 1;
	}
	return 0;
}
