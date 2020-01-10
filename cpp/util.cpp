#include "util.h"

using namespace std;

bool m_utils::isPrime(unsigned int v)
{
	if(v == 1)
		return true;
	if(v % 2 == 0)
		return false;

	for(int i = 3;i <= sqrt(static_cast<double>(v));++i)
	{
		if(v % i == 0)
			return false;
	}
	
	return true;
}

unsigned int m_utils::nextPrime(unsigned int n)
{
	unsigned int i = n;
	while(!isPrime(++i));
	return i;
}

int m_utils::setNonBlockingSoc(int fd)
{
	int old_option = fcntl(fd, F_GETFL);
	int new_option = old_option | O_NONBLOCK;
	fcntl(fd, F_SETFL, new_option);
	return old_option;
}

int m_utils::unblockConnect(const char* ip, short port, int times)
{
	sockaddr_in listen_addr;
	listen_addr.sin_family = AF_INET;
	listen_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &listen_addr.sin_addr);

	int conn_fd = socket(AF_INET, SOCK_STREAM, 0);
	int old_option = setNonBlockingSoc(conn_fd);

	if(connect(conn_fd, (struct sockaddr*)&listen_addr, sizeof(listen_addr)) == 0)
	{
		//如果直接连接成功，还原设置并直接返回
		fcntl(conn_fd, F_SETFL, old_option);
		return conn_fd;
	}
	else if(errno != EINPROGRESS)
	{
		perror("non_block connect");
		return -1;
	}

	fd_set writefds;
	struct timeval tv;

	FD_ZERO(&writefds);
	FD_SET(conn_fd, &writefds);
	tv.tv_usec = 0;
	tv.tv_sec = times;

	int ret = select(conn_fd, NULL, &writefds, NULL, &tv);
	if(ret <= 0)
	{
		printf("Connection time out\n");
		close(conn_fd);
		return -1;
	}

	if(!FD_ISSET(conn_fd, &writefds))
	{
		printf("no events on this socket\n");
		close(conn_fd);
		return -1;
	}

	int error = 0;
	socklen_t len = sizeof(error);

	if(getsockopt(conn_fd, SOL_SOCKET, SO_ERROR, &error, &len) < 0)
	{
		printf("getsockopt failed\n");
		close(conn_fd);
		return -1;
	}

	if(error != 0)
	{
		printf("connection failed after select with the error:%d\n", error);
		close(conn_fd);
		return -1;
	}

	printf("successful connect the ip and port\n");
	fcntl(conn_fd, F_SETFL, old_option);
	return conn_fd;
}	
