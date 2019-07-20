#include "../sysHeader.h"

int main(void)
{
	struct iovec iov[3];
	
	int fd = open("temp",O_RDWR);

	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	char v1[64];
	char v2[64];
	char v3[128];

	iov[0].iov_base = v1;
	iov[0].iov_len = 64;

	iov[1].iov_base = v2;
	iov[1].iov_len = 64;

	iov[2].iov_base = v3;
	iov[2].iov_len = 128;

	int totalreq = 256;

	int res = readv(fd,iov,3);
	
	if(res < totalreq)
		printf("read fewer bytes than requered\n");
	else
		printf("v1 = %s\nv2 = %s\nv3 = %s\n",v1,v2,v3);

	return 0;
}
