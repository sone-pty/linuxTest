#include "../sysHeader.h"

int main(int argc,char**argv)
{
	if(argc < 2)
	{
		printf("参数太少！");
		exit(1);
	}

	int fd = open(argv[1],O_RDWR);
	
	if(fd == -1)
	{
		perror("open:");
		exit(1);
	}

	int res = dup2(fd,STDIN_FILENO);

	if(res == -1)
	{
		perror("dup2:");
		exit(1);
	}

	close(fd);

	execl("./upper","upper",NULL);
	perror("execl:");
	return 0;
}
