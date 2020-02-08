#ifndef SOCKET_H_
#define SOCKET_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include "Log.h"
#include "InetAddress.h"

#define INVALID_FD -1

namespace sone
{

class Socket{
public:
	//默认创建非阻塞套接字
	explicit Socket(sa_family_t family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);
    explicit Socket(int fd, sa_family_t family);
    ~Socket();
    //返回原始fd
	int getFd() const { return _fd; }
	//为套接字绑定地址
	void bindAddr(const InetAddress& addr);
	//套接字开始监听
	void listen();
	//套接字连接函数(返回0表示成功连接，返回-1表示非阻塞套接字没有完成连接；如果连接失败直接退出)
	int connect(const InetAddress& addr);
	//套接字接受新连接，peer是对端的地址（如果调用正常返回新连接的fd，否则返回-1）
	int accept(InetAddress* peer);
	/*以下方法是设置该套接字的一些属性*/
	void setNoDelay(bool enable);
	void setKeepAlive(bool enable);
	void setNoBlock(bool enable);
	void setCloexec(bool enable);
private:
	int _fd = INVALID_FD;
	sa_family_t family;
};

}

#endif
