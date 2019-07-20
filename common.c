#include "common.h"
#include "sysHeader.h"

void sys_error(const char* buf,int exitcode)
{
	perror(buf);
	exit(exitcode);
}

void pr_mask()
{
	sigset_t now;
	sigprocmask(SIG_SETMASK,NULL,&now);

	printf("当前阻塞的信号:\n");
	
	int i;
	char buf[BUFFSIZE];
	
	for(i = 1;i < 32;i++)
	{
		if(sigismember(&now,i) == 1)
		{
			psignal(i,buf);
			printf("%s\n",buf);
		}
	}
}

ssize_t readn(int fd,void* buf,size_t n)
{
	ssize_t numRead;
	size_t totRead = 0;

	while(totRead < n)
	{
		numRead = read(fd,buf,n);

		if(numRead == 0)
			return numRead;
		else if(numRead == -1)
		{
			if(errno == EINTR)
				continue;
			else
				return -1;
		}

		totRead += numRead;
		buf += numRead;
	}

	return totRead;
}

ssize_t writen(int fd,void* buf,size_t n)
{
	ssize_t numWritten;
	size_t totWritten;

	while(totWritten < n)
	{
		numWritten = write(fd,buf,n - totWritten);

		if(numWritten == 0)
			return totWritten;
		else if(numWritten == -1)
		{
			if(errno == EINTR)
				continue;
			else
				return -1;
		}

		totWritten += numWritten;
		buf += numWritten;
	}

	return totWritten;
}
