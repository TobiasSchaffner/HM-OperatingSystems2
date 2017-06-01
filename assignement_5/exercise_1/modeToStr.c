#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define MODESTRSIZE 11

char modeString[MODESTRSIZE];

char *modeToStr(mode_t m)
{
	int idx = 0;

	memset(modeString, '\0', MODESTRSIZE);

	if( S_ISDIR(m))
		modeString[idx++] = 'd';
	else if ( S_ISLNK(m))
		modeString[idx++] = 'l';
	else
		modeString[idx++] = '-';

	if( S_IRUSR & m )
		modeString[idx++] = 'r';
	else
		modeString[idx++] = '-';

	if( S_IWUSR & m )
		modeString[idx++] = 'w';
	else
		modeString[idx++] = '-';

	if( S_IXUSR & m )
		modeString[idx++] = 'x';
	else
		modeString[idx++] = '-';

	if( S_IRGRP & m )
		modeString[idx++] = 'r';
	else
		modeString[idx++] = '-';

	if( S_IWGRP & m )
		modeString[idx++] = 'w';
	else
		modeString[idx++] = '-';

	if( S_IXGRP & m )
		modeString[idx++] = 'x';
	else
		modeString[idx++] = '-';

	if( S_IROTH & m )
		modeString[idx++] = 'r';
	else
		modeString[idx++] = '-';

	if( S_IWOTH & m )
		modeString[idx++] = 'w';
	else
		modeString[idx++] = '-';

	if( S_IXOTH & m )
		modeString[idx++] = 'x';
	else
		modeString[idx++] = '-';


	return modeString;
}
