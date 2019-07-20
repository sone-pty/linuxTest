#include "../sysHeader.h"

#define FILE_LEN 4096

typedef struct STU
{
	char name[20];
	char sex;
	int id;
}stu,*pstu;

int main(int argc,char** argv)
{
	pstu p;
	int fd;
	
	if(argc < 2)
	{
		printf("need more params\n");
		exit(1);
	}

	fd = open(argv[1],O_RDWR);

	if(fd == -1)
	{
		perror("open");
		exit(-1);
	}

	p = mmap(NULL,FILE_LEN,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	if(p == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	}

	close(fd);
	unlink(argv[1]);

	while(1)
	{
		printf("N.%d,Name:%s,Sex:%c\n",p->id,p->name,p->sex);
		sleep(1);
	}

	munmap(p,FILE_LEN);
	return 0;
}
