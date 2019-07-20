#include "IOHeader.h"

int main(void)
{
	int fd=open("temp",O_RDONLY);
	if(fd == -1)
	{
		perror("open error:");
		exit(1);
	}
	//lseek
	/*
	int off_set=lseek(fd,0,SEEK_END);
	if(off_set == -1)
	{
		perror("error for:");
		exit(1);
	}
	printf("file size:%dB\n",off_set);
	*/
	//stat
	struct stat data;
	int res=stat("temp",&data);
	if(res < 0)
	{
		perror("error for:");
		exit(1);
	}
	printf("file size:%dB\n",data.st_size);
	return 0;
}
