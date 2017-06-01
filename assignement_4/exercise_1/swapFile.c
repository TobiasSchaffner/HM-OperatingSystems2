#include<fcntl.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>

#define BUFSIZE 8192

void leave(const char *message) {
	printf(message);
	exit(1);
}

int check_permissions(const char* file_name, mode_t permissions) {
	int ret;
	struct stat file_stat;
	ret = stat(file_name, &file_stat);
	if (ret > 0) leave("Error getting file_stat");

	if(file_stat.st_uid != getuid()) {
		printf("File %s does not belong to you!\n", file_name);
		leave("Wrong owner!\n");
	}

	if ((file_stat.st_mode & permissions) == 0) {
		printf("File %s does not have the permissions needed!\n", file_name);
		if (permissions == S_IRUSR) leave("File not readable!\n");
		if (permissions == S_IWUSR) leave("File not writeble!\n");
		leave("Unknown permission error!");
	}

	return 0;
}

int move(const char *from_name, const char *to_name){
	int ret;
	check_permissions(from_name, S_IRUSR);
	check_permissions(to_name, S_IWUSR);

	char c[BUFSIZE];
	int from = open(from_name, O_RDONLY);
	int to = open(to_name, O_CREAT | O_WRONLY | O_TRUNC);
	while ((ret = read(from, &c, BUFSIZE)) > 0) {
		if (write(to, &c, ret) != ret) {
			close(from);
			close(to);
			perror(to_name);
			leave("Write Error!\n");
		}
	}
	close(from);
	close(to);

	if (ret < 0) {
		perror(from_name);
		leave("Read Error!\n");
	}

	return 0;
}

int main(int argc, char *argv[]) {

	const char *fdtmp = "/var/tmp/swap_tmp";

	if (argc != 3) leave("Please provide two files to be swaped!\n");

	move(argv[1], fdtmp);;
	move(argv[2], argv[1]);
	move(fdtmp, argv[2]);
	remove(fdtmp);

	return 0;
}
