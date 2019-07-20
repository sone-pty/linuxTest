#include "../sysHeader.h"

int main(void)
{
	fd_set rset;
	FD_ZERO(&rset);

	
	int fd = open("fifo",O_RDWR);
	
	if(fd < 0)
	{
		perror("open");
		exit(-1);
	}

	FD_SET(fd,&rset);
	FD_SET(STDIN_FILENO,&rset);

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;
	
	char buf[BUFFSIZE];

	int nums;
	
	while(1)
	{
		FD_ZERO(&rset);
		FD_SET(fd,&rset);
		FD_SET(STDIN_FILENO,&rset);

		nums = select(5,&rset,NULL,NULL,&tv);
		
		if(nums == -1)
		{
			perror("select");
			exit(-1);
		}

		if(nums == 0)
			continue;
		
		if(FD_ISSET(STDIN_FILENO,&rset))
		{
			printf("stdin has data\n");
			int recv = read(STDIN_FILENO,buf,BUFFSIZE);
			buf[recv] = 0;

			printf("recv from stdin:%s\n",buf);
		}

		if(FD_ISSET(fd,&rset))
		{
			printf("fifo has data\n");
			int recv = read(fd,buf,BUFFSIZE);
			buf[recv] = 0;

			printf("recv from fifo:%s\n",buf);
		}
	}

	return 0;
}
