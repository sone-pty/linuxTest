#include "../sysHeader.h"
#include <iostream>
#include <poll.h>

using namespace std;

int main(void)
{
	int port = 8989;
	const char* ip = "127.0.0.1";

	//服务器地址
	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);

	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(server_fd >= 0);

	if(connect(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
	{
		perror("connect");
		close(server_fd);
		return -1;
	}

	//为服务器socket和标准输入创建pollfd
	pollfd fds[2];
	fds[0].fd = STDIN_FILENO;
	fds[0].events = POLLIN;
	fds[0].revents = 0;
	fds[1].fd = server_fd;
	fds[1].events = POLLIN | POLLRDHUP;
	fds[1].revents = 0;

	char buf[64];
	int ret;

	while(1)
	{
		ret = poll(fds, 2, -1);
		if(ret < 0)
		{
			cout << "poll failed" << endl;
			break;
		}

		if(fds[1].revents & POLLRDHUP)
		{
			cout << "server close the connection" << endl;
			break;
		}
		//读取服务端数据
		if(fds[1].revents & POLLIN)
		{
			memset(buf, 0, 64);
			read(fds[1].fd, buf, 63);
			cout << buf << endl;
		}

		if(fds[0].revents & POLLIN)
		{
			memset(buf, 0, 64);
			int res = read(STDIN_FILENO, buf, 63);
			cout << "from stdin:" << res << endl;
			write(server_fd, buf, res);
		}
	}
	
	close(server_fd);
	return 0;
}
