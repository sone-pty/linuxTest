#include "Socket.h"
#include <netinet/tcp.h>
#include <fcntl.h>

namespace sone
{
	Socket::Socket(sa_family_t family, int type, int protocol)
	{
		_fd = socket(family, type | SOCK_CLOEXEC | SOCK_NONBLOCK, protocol);
		if(_fd == -1)
			SONE_LOG_ERR() << "socket创建失败";
		this->family = family;
	}

	Socket::~Socket()
	{
		if(close(_fd) < 0)
			SONE_LOG_ERR() << "套接字关闭失败";
	}

	void Socket::bindAddr(const InetAddress& addr)
	{		
		socklen_t len;
		if(family == AF_INET)
			len = sizeof(struct sockaddr_in);
		else
			len = sizeof(struct sockaddr_in6);
		if(bind(_fd, addr.Sockaddr(), len) < 0)
			SONE_LOG_ERR() << "绑定地址失败";
	}

	void Socket::listen()
	{
		if(::listen(_fd, SOMAXCONN) < 0)
		{
			SONE_LOG_ERR() << "listen调用失败";
			abort();
		}	
	}
	
	int Socket::connect(const InetAddress& addr)
	{
		socklen_t len = (family == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
		int ret = ::connect(_fd, addr.Sockaddr(), len);
		//如果是非阻塞套接字可能出现的情况
		if(ret == -1)
		{
			if(errno == EINPROGRESS || errno == EINTR)
				return -1;
			else
			{
				SONE_LOG_ERR() << "connect调用失败";
				abort();
			}
		}
		return 0;
	}

	int Socket::accept(InetAddress* peer)
	{
		int fd;
		socklen_t len;
		
		if(!peer)
			fd = ::accept(_fd, peer->writableSockaddr(), &len);
		else
			fd = ::accept(_fd, nullptr, nullptr);
		if(fd == -1 && (errno == EAGAIN || errno == EWOULDBLOCK))
			return -1;
		else if(fd == -1)
		{
			SONE_LOG_ERR() << "accept发生了错误";
			abort();
		}

		return fd;
	}
	
	void Socket::setNoDelay(bool enable)
	{
		int optval = enable ? 1 : 0;
		::setsockopt(_fd, IPPROTO_TCP, TCP_NODELAY, &optval, static_cast<socklen_t>(sizeof optval));
	}

	void Socket::setKeepAlive(bool enable)
	{
		int optval = enable ? 1 : 0;
		::setsockopt(_fd, SOL_SOCKET, SO_KEEPALIVE, &optval, static_cast<socklen_t>(sizeof optval));
	}

	void Socket::setNoBlock(bool enable)
	{
		int ret;
		if(enable)
		{
			int flags = ::fcntl(_fd, F_GETFL, 0);
			flags |= O_NONBLOCK;
			ret = ::fcntl(_fd, F_SETFL, flags);
		}
		else
		{
			int flags = ::fcntl(_fd, F_GETFL, 0);
			flags &= ~O_NONBLOCK;
			ret = ::fcntl(_fd, F_SETFL, flags);
		}
		if(ret == -1)
			SONE_LOG_ERR() << "改变套接字BLOCK标识失败";
	}

	void Socket::setCloexec(bool enable)
	{
		int ret;
		if(enable)
		{
			int flags = ::fcntl(_fd, F_GETFD, 0);
			flags |= FD_CLOEXEC;
			ret = ::fcntl(_fd, F_SETFD, flags);
		}
		else
		{
			int flags = ::fcntl(_fd, F_GETFD, 0);
			flags &= ~FD_CLOEXEC;
			ret = ::fcntl(_fd, F_SETFD, flags);
		}
		if(ret == -1)
			SONE_LOG_ERR() << "改变套接字CLOEXEC标识失败";
	}
}
