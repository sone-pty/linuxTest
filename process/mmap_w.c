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
	int i = 1;
	if(argc < 2)
	{
		printf("need more params\n");
		exit(1);
	}

	fd = open(argv[1],O_CREAT | O_RDWR,0777);
	
	if(fd == -1)
	{
		perror("open");
		exit(-1);
	}
	
	lseek(fd,FILE_LEN - 1,SEEK_SET);
	write(fd,"\0",1);

	p = mmap(NULL,FILE_LEN,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);

	if(p == MAP_FAILED)
	{
		perror("mmap");
		exit(-1);
	}
	
	close(fd);

	while(1)
	{
		p->id = i;
		sprintf(p->name,"pty-%d",i);
		if(i % 2 == 0)
			p->sex = 'm';
		else
			p->sex = 'w';
		sleep(1);
		i++;
	}
	
	munmap(p,FILE_LEN);
	return 0;
}
