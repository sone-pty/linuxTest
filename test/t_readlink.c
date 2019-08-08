#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE PATH_MAX

void te()
{
    printf("df\n");
}

int main(void)
{
	char buf[BUFFSIZE];
	char* path = "lntag";
	struct stat st;

	memset(buf,0,BUFFSIZE);
	printf("%d %d\n",'\0',buf[0]);
	int res = lstat(path,&st);
	if(res == -1)
	{
		perror("stat");
		exit(1);
	}

	if(!S_ISLNK(st.st_mode))
	{
		printf("lntag is not a slink\n");
		exit(1);
	}

	int nums = readlink(path,buf,BUFFSIZE - 1);
	buf[nums] = '\0';
	printf("lntag:%s\n",buf);
    te();
	return 0;
}
