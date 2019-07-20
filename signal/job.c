#include "../sysHeader.h"

int main(void)
{
	int i = 0;
	while(1)
	{
		i++;
		if(i == 10)
			kill(getpid(),SIGTSTP);
		if(i == 30)
			break;
		printf("i am working:%d for pid:%d\n",i,getpid());
		sleep(1);
	}
	return 0;
}
