#include "../sysHeader.h"
#include "../common.h"

int main(int argc,char** arg)
{
	if(argc < 3)
	{
		printf("need more params\n");
		exit(1);
	}

	int inputfd = open(arg[1],O_RDONLY);
	int outputfd = open(arg[2],O_RDWR | O_CREAT | O_TRUNC,0777);
	char buf[BUFFSIZE];
	int numread;

	if(inputfd < 0)
		sys_error("open for src",1);
	
	if(outputfd < 0)
		sys_error("open for dest",1);

	while((numread = read(inputfd,buf,BUFFSIZE)) > 0)
		if(write(outputfd,buf,numread) < 0)
			sys_error("write",1);

	return 0;
}
