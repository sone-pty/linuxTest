#include "../sysHeader.h"

int main(void)
{
	int fd = open("fifo",O_RDWR);
	
	if(fd == -1)
	{
		perror("open");
		exit(-1);
	}

	char buf[BUFFSIZE];
	int count = 10;
	int i;

	for(i = 0;i < count;i++)
	{
		sprintf(buf,"the NO.%d data",i);
		
		int err = write(fd,buf,strlen(buf));
		
		if(err == -1)
		{
			perror("write");
			exit(-1);
		}
		
		sleep(1);
	}

	return 0;
}
