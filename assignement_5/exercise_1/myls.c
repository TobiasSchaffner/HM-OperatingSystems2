#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<dirent.h>
#include<unistd.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>

#include "modeToStr.h"

void leave(const char *message) {
	printf(message);
	exit(1);
}

int main(int argc, char *argv[]) {
	if (argc != 2) leave("Please provide a directory to be shown!\n");

	int ret;
	DIR *directory = opendir(argv[1]);
	struct dirent *entry;
	struct stat file;
	char *buf = (char *) malloc(100 * sizeof(char));

	if(chdir(argv[1]) < 0) {
		perror("Error changing directory!");
		exit(1);
	}

	if (directory != 0) {
		while((entry = readdir(directory)) != NULL) {
			ret = lstat(entry->d_name, &file);
			if (ret != 0)
				leave("Error reading File!\n");
			if (entry->d_name[0] != '.') {
				printf(modeToStr(file.st_mode));
				printf(" %lu", file.st_nlink);
				printf(" %7s", getpwuid(file.st_uid)->pw_name);
				printf(" %7s", getgrgid(file.st_gid)->gr_name);
				printf(" %5lu", file.st_size);
				ret = strftime(buf, 100, "%b %d %H:%M", localtime(&file.st_mtim.tv_sec));
				printf(" %10s ", buf);
				printf(entry->d_name);
				if (modeToStr(file.st_mode)[0] == 'l') {
					printf(" -> ");
					ret = readlink(entry->d_name, buf, 100);
					if (ret < 0)
						leave("Error reading File!\n");
					buf[ret] = '\0';
					printf(buf);
				}
				printf("\n");
			}
		}
	} else perror("Could not open directory");
	closedir(directory);
	return 0;
}
