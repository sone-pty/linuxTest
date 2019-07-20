#include "../sysHeader.h"

int main(void)
{
	sigset_t all;
	siginfo_t info;
	int sig;
	pid_t child;

	sigfillset(&all);
	child = fork();

	if(child == -1)
	{
		perror("fork");
		exit(1);
	}

	if(child == 0)
	{	
		int count = 1;
		while(count <= 3)
		{
			printf("now child send SIGUSR1 to parent %d time\n",count);
			kill(getppid(),SIGUSR1);
			sleep(1);
			count++;
		}
	}
	else
	{
		sigprocmask(SIG_SETMASK,&all,NULL);

		while(1)
		{
			sig = sigwaitinfo(&all,&info);
			if(sig == SIGUSR1)
				printf("parent recv one SIGUSR1,and want next\n");
			else if(sig == SIGCHLD)
			{
				wait(NULL);
				exit(0);
			}
			else
			{
				perror("sigwaitinfo");
				exit(-1);
			}
		}
	}

	return 0;
}
