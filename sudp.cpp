#include "sysHeader.h"

int main(int argc,char** argv)
{
    int sock;
	if(argc < 2)
	{
		printf("err:need more arg\n");
		exit(-1);
	}

	int nums = atoi(argv[1]);
	char buf[409600];

	memset(buf,0,409600);
	int index = 0;
	for(index = 0;index < nums;index++)
		buf[index] = 'a';

	sockaddr_in ser,cli;
	ser.sin_family = AF_INET;
	ser.sin_port = 0;
	inet_aton("10.32.237.47",&ser.sin_addr);
    sock = socket(AF_INET,SOCK_DGRAM,0);
    bind(sock,(sockaddr*)&ser,sizeof(sockaddr));

    inet_aton("10.32.237.47",&cli.sin_addr);
    cli.sin_family = AF_INET;
    cli.sin_port = htons((unsigned short)9856);

    int res = sendto(sock,buf,nums,0,(sockaddr*)&cli,sizeof(sockaddr));
    return 0;
}
