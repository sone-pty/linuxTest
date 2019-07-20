#include "../sysHeader.h"

void proc(int sig)
{
	printf("sigint proc\n");
}

int main(void)
{
	signal(SIGINT,proc);
	sigset_t new,old;
	sigemptyset(&new);
	sigemptyset(&old);
	sigaddset(&new,SIGINT);

	sigprocmask(SIG_BLOCK,&new,&old);
	kill(getpid(),SIGINT);
	printf("已经发送了SIGINT信号\n");
	sleep(1);
	sigprocmask(SIG_SETMASK,&old,NULL);


	return 0;
}
