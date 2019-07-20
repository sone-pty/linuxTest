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

int main(int argc,char **argv)
{
	/*
	if(argc < 2)
	{
		printf("need more params\n");
		exit(1);
	}

	int res = mkfifo(argv[1],0777);

	if(res == -1)
	{
		perror("mkfifo");
		exit(1);
	}
	*/
	int fd = open("myfifo",O_WRONLY);
	
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	stu s;
	tea t;

	memset(&s,0,sizeof(s));
	memset(&t,0,sizeof(t));

	int i,res;
	for(i = 0;i < 5;i++)
	{	
		sleep(2);
		if(i % 2 == 0)
		{
			s.id = i;
			sprintf(s.name,"pty-%d",i);
			sprintf(s.info,"ptyinfo");
			
			res = write(fd,&s,sizeof(s));
			
			if(res == -1)
			{
				perror("write");
				exit(-1);
			}
		}
		else
		{
			t.id = i;
			sprintf(t.name,"pp-%d",i);
			t.sal = 1000;

			res = write(fd,&t,sizeof(t));

			if(res == -1)
			{
				perror("write");
				exit(-1);
			}
		}	
	}

	close(fd);
	
	return 0;
}
