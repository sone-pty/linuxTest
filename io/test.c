#include "IOHeader.h"
#include <wchar.h>

int main(int argc, char const *argv[])
{
	/*
	//if the O_APPEND flag setted,lseek wouldn't work normally

	char data[1024] = "new data\n";
	
	if(argc < 2)
	{
		printf("need more paramsn\n");
		exit(1);
	}

	int fd,res;
	int len;

	fd = open(argv[1],O_APPEND | O_RDWR);

	if(fd == -1)
	{
		perror("open");
		exit(-1);
	}

	len = lseek(fd,0,SEEK_END);
	printf("the file length:%d\n",len);
	
	res = lseek(fd,10,SEEK_SET);

	if(res == -1)
	{
		perror("lseek");
		exit(-1);
	}

	res = write(fd,data,strlen(data));

	if(res == -1)
	{
		perror("write");
		exit(-1);
	}
	close(fd);

	return 0;
	*/
	/*
	int fd = open("temp",O_CREAT | O_EXCL);

	if(fd == -1 && errno == EEXIST)
		printf("temp exist!\n");
	
	struct stat s;
	fstat(fd,&s);

	if(S_ISDIR(s.st_mode))
		printf("this is dir\n");
	int flag = fcntl(fd,F_GETFL);
	if(flag & S_IRUSR)
		printf("user can read\n");

	FILE* f = fopen("temp","w+");
	int fw = fwide(f,0);
	if(fw > 0)
		printf("wide char\n");
	else
		printf("byte char\n");
	printf("page size = %ld\n",sysconf(_SC_PAGESIZE));
	*/

	int fd = open("temp",O_RDWR | O_CREAT,0777);

	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	//unlink("temp");
	/*
	off_t new = lseek(fd,63,SEEK_END);
	
	printf("now the offset of the temp file is %ld\n",new);
	
	char buf[64];
	memset(buf,0,64);
	int numwrite = write(fd,buf,64);
	*/
	int numwrite = write(fd,"",1);
	printf("write %d bytes to the temp file now\n",numwrite);
	return 0;

}
