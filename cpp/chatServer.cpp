#include "../sysHeader.h"
#include <poll.h>
#include <iostream>
#include <vector>
#include <memory>
#include "util.h"

#define _GNU_SOURCE 1

using namespace std;

struct cli_data{
	typedef shared_ptr<cli_data> ptr;
	
	char buf[64];
	char* write_pos;
	int fd;
};

int main(void)
{
	
	int res;
	const char* ip = "127.0.0.1";
	int port = 8989;

	sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	inet_pton(AF_INET, ip, &server_addr.sin_addr);
	
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	assert(server_fd != -1);

	res = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	assert(res != -1);

	listen(server_fd, 5);

	vector<pollfd> fds;
	vector<cli_data::ptr> clidatas;

	fds.push_back(pollfd());
	fds[0].fd = server_fd;
	fds[0].events = POLLIN;
	fds[0].revents = 0;

	while(1)
	{
		int n = poll(&(*fds.begin()), fds.size(), -1);
		int fds_size = fds.size();

		if(n < 0)
		{
			cout << "poll failed" << endl;
			break;
		}

		for(int i = 0;i < fds_size;++i)
		{
			//新连接
			if(fds[i].fd == server_fd && fds[i].revents & POLLIN)
			{
				int con_fd = accept(server_fd, NULL, NULL);
				assert(con_fd != -1);
				m_utils::setNonBlockingSoc(con_fd);				

				fds.push_back(pollfd());
				fds[fds.size() - 1].fd = con_fd;
				fds[fds.size() - 1].events = POLLIN | POLLRDHUP;
				fds[fds.size() - 1].revents = 0;

				clidatas.push_back(make_shared<cli_data>(cli_data()));
				clidatas[clidatas.size() - 1]->fd = con_fd;
				memset(clidatas[clidatas.size()- 1]->buf, 0, 64);
				cout << "新客户端" << con_fd << "连接了" << endl;
			}

			if(fds[i].fd != server_fd && (fds[i].revents & POLLIN))
			{
				memset(clidatas[i - 1]->buf, 0, 64);
				res = recv(fds[i].fd, clidatas[i - 1]->buf, 63, 0);
				clidatas[i - 1]->buf[res - 1] = '\0';
				
				if(res - 1 > 0)
				{
					//通知发送给其他客户端
					for(int j = 0;j < (int)clidatas.size();++j)
					{
						if(clidatas[j]->fd == fds[i].fd)
							continue;
						//write(clidatas[j]->fd, temp.c_str(), temp.size());
						fds[j + 1].events |= ~POLLIN;
						fds[j + 1].events |= POLLOUT;
						clidatas[j]->write_pos = clidatas[i - 1]->buf;
					}
				}
			}

			if(fds[i].fd != server_fd && (fds[i].revents & POLLOUT))
			{
				if(!clidatas[i - 1]->write_pos)
					continue;
				write(clidatas[i - 1]->fd, clidatas[i - 1]->write_pos, strlen(clidatas[i - 1]->write_pos));
				clidatas[i - 1]->write_pos = NULL;
				fds[i].events |= ~POLLOUT;
				fds[i].events |= POLLIN;
			}

			if(fds[i].fd != server_fd && fds[i].revents & POLLRDHUP)
			{

			}
		}
	}

	close(server_fd);
	return 0;
}
