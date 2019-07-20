#include "../sysHeader.h"

int main(void)
{
	/*
	pid_t pid;
	int res;
	int fd[2];

	res = pipe(fd);
	if(res == -1)
	{
		perror("pipe");
		exit(1);
	}

	pid = fork();

	if(pid > 0)
	{
		char data[1024] = "this is data\n";
		close(fd[0]);
		sleep(3);
		res = write(fd[1],data,strlen(data));
		if(res == -1 && errno == EPIPE)
		{
			printf("catch SIGPIPE signal\n");
			exit(1);
		}
		close(fd[1]);
		wait(NULL);
	}
	else if(pid == 0)
	{
		char resv[1024];
		close(fd[1]);
		close(fd[0]);
	}
	else
	{
		perror("fork");
		exit(-1);
	}*/
	
	int pid;

	pid = fork();

	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		execl("child","arg0",(char*)0);
		printf("child process\n");
	}
	else
	{
		wait(NULL);
		printf("parent process\n");
	}
}
