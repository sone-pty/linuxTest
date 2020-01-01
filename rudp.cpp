#include "sysHeader.h"
#include "std.h"

int main(void)
{
	int sock;
	sockaddr_in cli;
	memset(&cli,0,sizeof(cli));

	inet_aton("10.32.237.47",&cli.sin_addr);
	cli.sin_family = AF_INET;
	cli.sin_port = htons(9856);

	sock = socket(AF_INET,SOCK_DGRAM,0);
	cout << "sock = " << sock << endl;

	int res = bind(sock,(struct sockaddr*)&cli,sizeof(struct sockaddr));
	cout << "bind res = " << res << endl;
	if(res == -1)
		perror("bind");
	
	char buf[409600];
	memset(buf,0,409600);
	while(1)
	{
		res = recvfrom(sock,buf,409600,0,NULL,NULL);
		cout << "recvfrom res = " << res << endl;
	}

	return 0;
}
