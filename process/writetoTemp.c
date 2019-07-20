#include "../sysHeader.h"
#define FILENAME "temp"
int main(void)
{
	//共享文件描述符
	int fd;
	//子进程id
	int pid_c;

	fd = open(FILENAME,O_RDWR | O_APPEND);
	
	if(fd == -1)
	{
		perror("open");
		exit(-1);
	}

	pid_c = fork();

	if(pid_c == 0)
	{
		//子进程
		int n = 10;
		char buf[1024] = "child process write data to the file\n";
		while(n-- > 0)
		{
			int len = write(fd,buf,strlen(buf));
			if(len != strlen(buf))
			{
				perror("error");
				exit(-1);
			}
		}
		close(fd);
	}
	else if(pid_c > 0)
	{
		//父进程
		int n = 20;
		char buf[1024] = "parent process write data to the file\n";
		sleep(5);
		while(n-- > 0)
		{
			int len = write(fd,buf,strlen(buf));
			if(len != strlen(buf))
			{
				perror("error");
				exit(-1);
			}
		}
	}
	return 0;
}
