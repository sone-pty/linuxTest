#include "../sysHeader.h"


void printsigset(const sigset_t* set)
{
	int i;
	for(i = 1;i < 32;i++)
	{
		if(sigismember(set,i) == 1)
			putchar('1');
		else
			putchar('0');
		printf(" ");
	}
	printf("\n");
}

void usr(int p)
{
	sigset_t old;
	sigprocmask(NULL,NULL,&old);
	printsigset(&old);
	return;
}

int main(void)
{
	/*
	sigset_t new,old;
	
	sigprocmask(NULL,NULL,&old);

	printsigset(&old);

	if(signal(SIGUSR1,usr) == SIG_ERR)
	{
		perror("signal");
		exit(1);
	}
	
	kill(getpid(),SIGUSR1);
	sigprocmask(NULL,NULL,&old);
	printsigset(&old);
	pause();
	*/
	pid_t pid;
	sigset_t new,old;
	sigset_t now;

	sigemptyset(&new);
	sigaddset(&new,SIGUSR1);
	sigprocmask(SIG_BLOCK,&new,&old);
	sigprocmask(NULL,NULL,&now);
	
	printf("parent now = %x\n",&now);
	printf("parent mask:");
	printsigset(&now);

	pid = fork();
	if(pid < 0)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		sigprocmask(NULL,NULL,&now);
		printf("child now = %x\n",&now);
		printf("child mask:");
		printsigset(&now);
		sigaddset(&now,SIGUSR2);
		printsigset(&now);
	}
	else
	{
		wait(NULL);
		sigprocmask(SIG_SETMASK,&old,NULL);
		printsigset(&now);
	}

	return 0;
}
