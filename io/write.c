#include "IOHeader.h"

int main(void)
{
	int fd=open("temp",O_WRONLY);
	if(fd == -1)
	{
		perror("open error:");
		exit(1);
	}
	char buf[1024]="HELLOWORLD!\n";
	int i=100;
	while(i-->0)
		write(fd,buf,strlen(buf));
	return 0;
}
