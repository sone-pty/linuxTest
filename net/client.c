#include "../sysHeader.h"

int main(void)
{
	struct sockaddr_in servaddr;//服务器端地址
    struct sockaddr_in clientAddr;//客户端地址
    int sockfd; 
    int clientAddrLen = sizeof(clientAddr);
    char ipAddress[1024];//保存点分十进制的ip地址
    
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(8888);  
    inet_aton("127.0.0.1",&servaddr.sin_addr.s_addr);

	memset(&clientAddr,0,clientAddrLen);
	clientAddr.sin_family = AF_INET;
	clientAddr.sin_port = htons(7765);
	inet_aton("127.0.0.1",&clientAddr.sin_addr.s_addr);
	bind(sockfd,(struct sockaddr*)&clientAddr,clientAddrLen);
 
    connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));//向服务器端发起连接请求
	struct sockaddr_in temp;
    socklen_t addrlen;
    memset(&temp,0,sizeof(temp));
	int res = getsockname(sockfd,(struct sockaddr*)&temp,&addrlen);
	printf("sockfd bind addr:%s,port:%d\n",inet_ntoa(temp.sin_addr),ntohs(temp.sin_port));
	while(1);
    return 0;
}
