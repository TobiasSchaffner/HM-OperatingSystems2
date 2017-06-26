/*
 *  ioctl.c - the process to use ioctl's to control the kernel module
 *
 *  Until now we could have used cat for input and output.  But now
 *  we need to do ioctl's, which require writing our own process.
 */

/* 
 * device specifics, such as ioctl numbers and the
 * major device file. 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>		/* open */
#include <unistd.h>		/* exit */
#include <sys/ioctl.h>		/* ioctl */

#include "openFileSearch.h"

/* 
 * Main - Call the ioctl functions 
 */
int main(int argc, char *argv[]) {
	struct ofs_result* result;
	int file_desc, ret_val, ofs_id, result_count;
	char c;

	if (argc != 4) {
		printf("Usage: ioctl <mode> <param> <Number of results>\n");
		printf("Modes:\n");
		printf("    pid           Get all open files for a Process id\n");
		printf("      param:      Integer: ID of the process\n");
		printf("\n");
		printf("    uid           Get all open files for a User\n");
		printf("      param:      Integer: UID of the user\n");
		printf("\n");
		printf("    owner         Get all open files that are owned by a given user\n");
		printf("      param:      Integer: UID of the owner\n");
		printf("\n");
		printf("    name          Get all open files with the given name\n");
		printf("      param:      String up to 64 byte: Name of the file\n");
		printf("\n");
		printf("Number of Results: The maximum number of results you want to get.\n");
		exit(1);
	}

	file_desc = open(DEVICE_FILE_NAME, 0);
	if (file_desc < 0) {
		printf("Can't open device file: %s\n", DEVICE_FILE_NAME);
		exit(-1);
	}

	if (strcmp(argv[1], "pid") == 0) {
		ofs_id = OFS_PID;
		ret_val = ioctl(file_desc, ofs_id, atoi(argv[2]));
	}
	else if (strcmp(argv[1], "uid") == 0) {
		ofs_id = OFS_UID;
		ret_val = ioctl(file_desc, ofs_id, atoi(argv[2]));
	}
	else if (strcmp(argv[1], "owner") == 0) {
		ofs_id = OFS_OWNER;
		ret_val = ioctl(file_desc, ofs_id, atoi(argv[2]));
	}
	else if (strcmp(argv[1], "name") == 0) {
		ofs_id = OFS_NAME;
		ret_val = ioctl(file_desc, ofs_id, argv[2]);
	} else {
		close(file_desc);
		printf("Unknown mode!\n");
		exit(1);
	}
	if (ret_val < 0) {
		close(file_desc);
		printf("ioctl failed:%d\n", ret_val);
		exit(1);
	}

	result_count = atoi(argv[3]);
	if (result_count < 1 ) {
		close(file_desc);
		printf("Please provide a valid Number > 0 for Number of results\n", ret_val);
		exit(1);
	}

	ret_val = read(file_desc, results, result_count);
	if (ret_val < 0) {
		close(file_desc);
		printf("There is something wrong!\nHave you searched before reading and are your arguments correct?\n");
		exit(1);
	}
	if (ret_val < result_count)
		result_count = ret_val;

	for (int i = 0; i < result_count; i++) {
	result = &results[i];
	printf("Result %d {\n    pid: %d\n    uid: %d\n    owner: %d\n    permissions: %3o\n    name: %s\n    fsize: %d\n    inode_no: %lu\n}\n",
		i,
		result->pid,
		result->uid,
		result->owner,
		result->permissions&0777,
		result->name,
		result->fsize,
		result->inode_no);
	}

	close(file_desc);
}
