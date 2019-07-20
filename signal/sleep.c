#include "../sysHeader.h"

void printsigset(const sigset_t* set)
{
	int i;
	char buf[1024];
	memset(buf,0,sizeof(buf));
	for(i = 1;i < 32;i++)
	{
		if(sigismember(set,i) == 1)
		{
			psignal(i,buf);
			printf("%s ",buf);
		}
	}
	printf("\n");
}

void pr_mask()
{
	sigset_t now;
	sigprocmask(SIG_SETMASK,NULL,&now);
	printsigset(&now);
}

static void wakeup(int signo)
{
	pr_mask();
}

unsigned int sleep(unsigned int sec)
{
	struct sigaction oact,nact;
	sigset_t nmask,omask,susmask;
	unsigned int res;

	sigemptyset(&nmask);
	sigemptyset(&susmask);

	//block SIGALRM
	sigaddset(&nmask,SIGALRM);
	sigprocmask(SIG_BLOCK,&nmask,&omask);

	//set action
	nact.sa_handler = wakeup;
	sigemptyset(&nact.sa_mask);
	nact.sa_flags = 0;
	sigaction(SIGALRM,&nact,&oact);

	alarm(sec);
	susmask = omask;

	sigdelset(&susmask,SIGALRM);
	sigsuspend(&susmask);
	
	pr_mask();
	res = alarm(0);

	//reback
	sigprocmask(SIG_SETMASK,&omask,NULL);
	sigaction(SIGALRM,&oact,NULL);

	return res;
}

void usr(int signo)
{
	pr_mask();
}

int main(void)
{
	struct sigaction nact;
	nact.sa_handler = usr;
	sigemptyset(&nact.sa_mask);
	nact.sa_flags = 0;
	pause();
	return 0;
}
