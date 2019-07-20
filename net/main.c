#include "../sysHeader.h"

int main(void)
{
	struct hostent* hostnet = gethostbyname("smtp.163.com");
	if(hostnet != NULL)
	{
		sockaddr_in serveraddr;
		memset(&serveraddr,0,sizeof(serveraddr));
	}
}
