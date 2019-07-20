#include "../sysHeader.h"

int main(int argc,char** arg)
{
	int fdin,fdout;
	void* src,*dest;
	struct stat fs;

	if(argc < 3)
	{
		printf("need more params\n");
		exit(1);
	}

	fdin = open(arg[1],O_RDONLY);
	
	if(fdin < 0)
	{
		perror("open for fdin");
		exit(-1);
	}
	
	umask(0);
	fdout = open(arg[2],O_CREAT | O_RDWR | O_TRUNC,0777);

	if(fdout < 0)
	{
		perror("open for fdout");
		exit(-1);
	}

	int err = fstat(fdin,&fs);

	if(err < 0)
	{
		perror("fstat");
		exit(-1);
	}

	err = ftruncate(fdout,fs.st_size);
	if(err < 0)
	{
		perror("ftruncate");
		exit(-1);
	}

	off_t len = fs.st_size;
	src = mmap(0,len,PROT_READ,MAP_SHARED,fdin,0);
	dest = mmap(0,len,PROT_READ | PROT_WRITE,MAP_SHARED,fdout,0);
	
	if(src == MAP_FAILED)
	{
		perror("mmap for src");
		exit(-1);
	}

	if(dest == MAP_FAILED)
	{
		perror("mmap for dest");
		exit(-1);
	}

	memcpy(dest,src,len);
	munmap(src,4096);
	munmap(dest,4096);

	return 0;
}
