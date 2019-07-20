#include "../sysHeader.h"

int main(void)
{
	int fd;
	char* p;
	fd = open("temp",O_RDWR);

	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	int len = lseek(fd,0,SEEK_END);
	p = mmap(NULL,len,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	if(p == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	int i;
	char c = 'A';
	for(i = 0;i < len;i++)
		*(p + i) = c + i;
	
	int res = munmap(p,len);
	if(res == -1)
	{
		perror("munmap");
		exit(-1);
	}

	return 0;
}
