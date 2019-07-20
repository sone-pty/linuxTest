#include "../sysHeader.h"

static volatile int flag;
static sigset_t zero,new,old;

void syserr(const char* str,int exitcode)
{
	perror(str);
	exit(exitcode);
}

void usr()
{
	flag = 1;
}

void tell_wait()
{
	if(signal(SIGUSR1,usr) == SIG_ERR)
		syserr("signal",-1);
	if(signal(SIGUSR2,usr) == SIG_ERR)
		syserr("signal",-1);
	
	sigemptyset(&zero);
	sigemptyset(&new);
	sigaddset(&new,SIGUSR1);
	sigaddset(&new,SIGUSR2);

	sigprocmask(SIG_BLOCK,&new,&old);
}

void wait_parent()
{
	while(flag == 0)
		sigsuspend(&zero);

	flag = 0;

	sigprocmask(SIG_SETMASK,NULL,&old);
}

void tell_parent(pid_t pid)
{
	kill(pid,SIGUSR1);
}

void tell_child(pid_t pid)
{
	kill(pid,SIGUSR2);
}

void wait_child()
{
	while(flag == 0)
		sigsuspend(&zero);

	flag = 0;

	sigprocmask(SIG_SETMASK,NULL,&old);
}

int main(void)
{
	tell_wait();

	pid_t pid;

	pid = fork();

	if(pid < 0)
		syserr("fork",-1);
	else if(pid == 0)
	{
		printf("first child\n");
		tell_parent(getppid());
	}
	else if(pid > 0)
	{
		wait_child();
		printf("later parent\n");
		wait(NULL);
	}
	return 0;
}
