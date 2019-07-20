#include "../sysHeader.h"
#define PORT 8888

int main(void)
{
	struct sockaddr_in lisaddr;
	int lsoc,cli;

	memset(&lisaddr,0,sizeof(lisaddr));
	lisaddr.sin_family = AF_INET;
	lisaddr.sin_port = htons(PORT);
	inet_aton("127.0.0.1",&lisaddr.sin_addr.s_addr);

	lsoc = socket(AF_INET,SOCK_STREAM,0);
	bind(lsoc,(struct sockaddr*)&lisaddr,sizeof(lisaddr));
	listen(lsoc,5);

	struct sockaddr_in temp,peer;
	socklen_t addrlen,plen;
	memset(&temp,0,sizeof(temp));
	int res = getsockname(lsoc,(struct sockaddr*)&temp,&addrlen);

	if(res == -1)
	{
		printf("getsockname failed\n");
		exit(0);
	}

	printf("lsoc bind addr:%s,port:%d\n",inet_ntoa(temp.sin_addr),ntohs(temp.sin_port));
	
	while(1)
	{
		struct sockaddr_in ac;
		int len;
		cli = accept(lsoc,NULL,0);
		//printf("from accept the clisock bind addr:%s,port:%d\n",inet_ntoa(ac.sin_addr),ntohs(ac.sin_port));
		memset(&temp,0,sizeof(temp));
		memset(&peer,0,sizeof(peer));
		if(cli == -1)
		{
			perror("accept");
			exit(0);
		}

		int res = getsockname(cli,(struct sockaddr*)&temp,&addrlen);
		
		if(res == -1)
		{
			printf("getsockname failed\n");
			exit(0);
		}
		
		res = getpeername(cli,(struct sockaddr*)&peer,&plen);

		if(res == -1)
		{
			printf("getpeername failed\n");
			exit(0);
		}
		
		printf("clisock bind addr:%s,port:%d\n",inet_ntoa(temp.sin_addr),ntohs(temp.sin_port));
		printf("peersock bind addr:%s,port:%d\n",inet_ntoa(peer.sin_addr),ntohs(peer.sin_port));
	}

	return 0;
}
