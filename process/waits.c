#include "../sysHeader.h"

int main(int argc, char const *argv[])
{
	pid_t pid;
	int stat;
	int n = 3;
	int loop = 3;
	while(n-- > 0)
	{
		pid = fork();
		if(pid == 0)
			break;
	}
	if(pid == 0)
	{
		//child process exec for 3 loops
		while(loop-- > 0)
		{
			printf("i am child%d,work for NO.%d loop\n",getpid(),loop);
			sleep(3);
		}
	}
	else if(pid > 0)
	{
		//parent process
		pid_t pid_c;
		while(1)
		{
			printf("i am parent\n");
			//wait for the same group process
			pid_c = waitpid(0,&stat,WNOHANG);
			if(pid_c == -1)
				printf("error for waitpid\n");
			else if(pid_c == 0)
				printf("state not changed\n");
			else
			{
				if(WIFEXITED(stat))
				{
					printf("the process:NO.%d is terminated normally,the return status is %d\n",pid_c,WEXITSTATUS(stat));
				}
			}
			sleep(1);
		}
	}
	return 1;
}
