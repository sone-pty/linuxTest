#include "../sysHeader.h"

int main(void)
{
	int fd[2];
	int res = pipe(fd);
	pid_t pid;
	char data[1024]="parent process data";
	char recv[1024];
	
	printf("pipe maximum:%d\n",fpathconf(fd[0],_PC_PIPE_BUF));
	if(res == -1)
	{
		perror("pipe");
		exit(1);
	}

	res = fork();
	if(res == -1)
	{
		perror("fork");
		exit(1);
	}

	if(res > 0)
	{
		//in parent,close read fd
		close(fd[0]);
		sleep(3);
		write(fd[1],data,strlen(data));
		//wait for child
		close(fd[1]);
		wait(NULL);
	}
	else if(res == 0)
	{
		//in child
		close(fd[1]);
		memset(recv,0,sizeof(recv));

		//set nonblock
		int flag = fcntl(fd[0],F_GETFL);
		flag |= O_NONBLOCK;
		fcntl(fd[0],F_SETFL,flag);
			
		int len;
		while((len = read(fd[0],recv,sizeof(recv))) <= 0)
		{
			if(len == -1)
			{
				if(errno == EAGAIN)
				{
					sleep(1);
					printf("no data recv,continue\n");
					continue;
				}
				else
				{
					perror("read");
					exit(1);
				}
			}
			else if(len == 0)
				break;
		}
		printf("get data from parent process : %s\n",recv);
	}
	
	return 0;
}
