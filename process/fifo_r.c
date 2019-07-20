#include "../sysHeader.h"

typedef struct STU
{
	char id;
	char name[10];
	char info[20];
}stu,*pstu;

typedef struct TEACHER
{
	char id;
	char name[10];
	int sal;
}tea,*ptea;

int main(int argc,char**argv)
{
	/*
	if(argc < 2)
	{
		printf("need more params\n");
		exit(1);
	}
	*/
	int res,fd;
	fd = open("myfifo",O_RDONLY);

	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	char recv[1024];
	int i = 1;
	while(i <= 5)
	{
		res = read(fd,recv,sizeof(recv));

		if(res == -1)
		{
			perror("read");
			exit(-1);
		}

		printf("N.%d:recv data for len:%d\n",i,res);
		i++;
	}

	return 0;
}
